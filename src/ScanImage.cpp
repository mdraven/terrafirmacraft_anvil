
#include "ScanImage.hpp"

#include <FL/Fl.H>
#include <FL/Fl_PNG_Image.H>

#include <cassert>
#include <cstring>

// Размер окошка наковальни + окошка с инвентарём
static const int g_anvil_inventory_width = 208;
static const int g_anvil_inventory_height = 204;

// Смещение стрелок от верхнего края окна
static const int g_red_offset = 103;
static const int g_green_offset = 108;

// Смещение линейки и её длинна
static const int g_left_offset = 27;
static const int g_scale_len = 153;

struct ScanImage::Private {
    Status m_status;
    int m_scale = 1;
    std::unique_ptr<Fl_PNG_Image> m_markers;
    int m_red_score;
    int m_green_score;

    bool matchImage(const Fl_PNG_Image* image, int left, int top, int scale, bool is_green);
    void clear();

    // widht, height -- размер скриншота
    // scale -- размер интерфейса. normal - 2
    static void getCoord(int width, int height, int scale,
                         int& scale_left, int& scale_right,
                         int& red_y, int& green_y);
    static bool isTransparent(const Fl_PNG_Image* image, int x, int y);
    static bool isEqual3(const Fl_PNG_Image* a, int a_x, int a_y,
                         const Fl_PNG_Image* b, int b_x, int b_y);

    Private();
};

ScanImage::Private::Private() {
    clear();
    m_markers.reset(new Fl_PNG_Image("markers.png"));
}

bool ScanImage::Private::matchImage(const Fl_PNG_Image* image, int left, int top, int scale, bool is_green) {
    int marker_h = m_markers->h();
    int marker_w = m_markers->w();
    int marker_d = m_markers->d();

    assert(marker_d == 4);

    int marker_x = is_green ? 0 : (marker_w / 2);

    for(int y = 0; y < marker_h; ++y) {
        for(int x = 0; x < marker_w / 2; ++x) {
            if(!isTransparent(m_markers.get(), x + marker_x, y)) {
                for(int scale_x = 0; scale_x < scale; ++scale_x)
                    for(int scale_y = 0; scale_y < scale; ++scale_y)
                        if(!isEqual3(m_markers.get(), x + marker_x, y, image, x * scale + left + scale_x, y * scale + top + scale_y))
                            return false;
            }
        }
    }

    return true;
}

bool ScanImage::Private::isTransparent(const Fl_PNG_Image* image, int x, int y) {
    if(image->d() != 4)
        return false;

    return image->array[(image->w() * y + x) * image->d() + 3] == 0;
}

bool ScanImage::Private::isEqual3(const Fl_PNG_Image* a, int a_x, int a_y,
                                  const Fl_PNG_Image* b, int b_x, int b_y) {
    const int depth = 3;

    if(a->d() < depth || b->d() < depth)
        return false;

    return std::memcmp(&a->array[(a->w() * a_y + a_x) * a->d()],
                       &b->array[(b->w() * b_y + b_x) * b->d()],
                       depth) == 0;
}

void ScanImage::Private::clear() {
    m_red_score = -1;
    m_green_score = -1;
    m_status = Status::initial;
}

void ScanImage::Private::getCoord(int width, int height, int scale,
                                  int& scale_left, int& scale_right,
                                  int& red_y, int& green_y) {
    int window_x = (width / 2) - (g_anvil_inventory_width * scale / 2);
    int window_y = (height / 2) - (g_anvil_inventory_height * scale / 2);

    scale_left = window_x + g_left_offset * scale;
    scale_right = scale_left + g_scale_len * scale;

    red_y = window_y + g_red_offset * scale;
    green_y = window_y + g_green_offset * scale;
}

ScanImage::ScanImage()
    : m_p(new Private())
{}

ScanImage::~ScanImage() = default;

bool ScanImage::openImage(const std::string& filename) {
    m_p->clear();

    std::unique_ptr<Fl_PNG_Image> png(new Fl_PNG_Image(filename.c_str()));

    if(png->fail() != 0 || png->d() != 3) {
        m_p->m_status = Status::cannot_open;
        return false;
    }

    const int scale = m_p->m_scale;

    int scale_left;
    int scale_right;
    int red_y;
    int green_y;

    m_p->getCoord(png->w(), png->h(), scale, scale_left, scale_right,
                  red_y, green_y);

    int red_x = -1;
    int green_x = -1;

    for(int i = scale_left; i < scale_right; ++i) {
        if(m_p->matchImage(png.get(), i, red_y, scale, false))
            red_x = i;

        if(m_p->matchImage(png.get(), i, green_y, scale, true))
            green_x = i;
    }

    if (red_x == -1 || green_x == -1) {
        m_p->m_status = Status::is_not_anvil;
        return false;
    }

    m_p->m_red_score = (red_x - scale_left) / scale;
    m_p->m_green_score = (green_x - scale_left) / scale;
    m_p->m_status = Status::ok;

    return true;
}

ScanImage::Status ScanImage::getStatus() const {
    return m_p->m_status;
}

bool ScanImage::getScores(int& red_score, int& green_score) const {
    if(m_p->m_status != Status::ok)
        return false;

    red_score = m_p->m_red_score;
    green_score = m_p->m_green_score;

    return true;
}

void ScanImage::setScale(int scale) {
    if(scale < 1)
        return;

    m_p->m_scale = scale;
}

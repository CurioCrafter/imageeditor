#pragma once

#include <QString>
#include <QImage>

namespace core {

// Forward declaration for Filter class
class Filter {
public:
    virtual ~Filter() = default;
    virtual QString getName() const = 0;
    virtual QImage apply(const QImage& input) = 0;
    virtual bool supportsPreview() const = 0;
    virtual QImage generatePreview(const QImage& input, const QSize& size) = 0;
};

} // namespace core

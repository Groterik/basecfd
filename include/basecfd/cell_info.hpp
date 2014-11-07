#ifndef BASECFD_CELL_INFO_GUARD_H
#define BASECFD_CELL_INFO_GUARD_H

#include <basecfd/definitions.hpp>
#include <basecfd/element.hpp>

#include <bitset>

namespace basecfd
{

class CellInfo
{
public:
    enum Type
    {
        INNER,
        BORDER,
        GHOST,
        BOUNDARY,

        CELL_TYPE_COUNT
    };

    explicit CellInfo(Type type = INNER, color_t color = 0) : t(type), c(color) {}

    Type type() const
    {
        return t;
    }

    color_t color() const
    {
        return c;
    }

    ElementType element() const
    {
        return e;
    }

    void type(Type value)
    {
        t = value;
    }

    void color(color_t value)
    {
        c = value;
    }

    void element(ElementType value)
    {
        e = value;
    }

private:
    Type t;
    color_t c;
    ElementType e;
};

} // namespace basecfd

#endif //BASECFD_CELL_INFO_GUARD_H

#ifndef BASECFD_ELEMENT_GUARD_H
#define BASECFD_ELEMENT_GUARD_H

namespace basecfd
{

/// Element types enumeration
enum ElementType
{
    WrongElement = -1, Point = 0, Line, Quadr, Unconsistent2D, Unconsistent3D, ElementsCount
};

} // namespace basecfd

#endif //BASECFD_ELEMENT_GUARD_H

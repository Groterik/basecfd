#ifndef BASECFD_GRID_INFO_GUARD_H
#define BASECFD_GRID_INFO_GUARD_H

#include <basecfd/definitions.hpp>
#include <basecfd/vector.hpp>
#include <basecfd/element.hpp>

#include <utility>
#include <array>

namespace basecfd
{

class GridInfo
{
public:
    typedef std::pair<Vector, Vector> Volume;
    typedef std::array<idx_t, ElementsCount> ElementsDistribution;
    GridInfo(Volume env, Volume characteristic, const ElementsDistribution& distrib)
        : vol(env), chr(characteristic), distrib(distrib)
    {}

    Volume envelopingSpace() const;
    Volume characteristicVolume() const;

    idx_t elements(ElementType type) const;

private:
    Volume vol;
    Volume chr;
    std::array<idx_t, ElementsCount> distrib;
};

} // namespace basecfd

#endif //BASECFD_GRID_INFO_GUARD_H

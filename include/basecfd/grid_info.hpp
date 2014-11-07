#ifndef BASECFD_GRID_INFO_GUARD_H
#define BASECFD_GRID_INFO_GUARD_H

#include <basecfd/definitions.hpp>
#include <basecfd/vector.hpp>
#include <basecfd/element.hpp>
#include <basecfd/cell_info.hpp>

#include <utility>
#include <array>
#include <vector>
#include <limits>

namespace basecfd
{

class GridInfo
{
public:
    typedef std::pair<Vector, Vector> Volume;
    typedef std::array<idx_t, ELEMENT_TYPE_COUNT> ElementsDistribution;
    GridInfo(Volume env, Volume cell_overall_dim, const ElementsDistribution& elements_distrib)
        : vol(env), cell(cell_overall_dim), elems(elements_distrib) {}
    GridInfo()
    {
        elems.fill(0);
        const auto min_value = std::numeric_limits<typename Volume::first_type::value_type>::max();
        const auto max_value = std::numeric_limits<typename Volume::second_type::value_type>::min();
        cell.first = vol.first = Vector(min_value, min_value, min_value);
        cell.second = vol.second = Vector(max_value, max_value, max_value);
    }

    Volume envelopingSpace() const
    {
        return vol;
    }
    Volume cellOverallDimension() const
    {
        return cell;
    }

    idx_t elements(ElementType type) const
    {
        return elems[type];
    }

    idx_t types(CellInfo::Type type) const
    {
        return tps[type];
    }

    idx_t colors(color_t color) const
    {
        static_assert(std::is_unsigned<color_t>::value, "Signed color");
        return clrs[color];
    }

    void add(CellInfo info)
    {
        ++elems[info.element()];
        ++tps[info.type()];
        ++clrs[info.color()];
    }

    void cellOverallDimension(Volume value)
    {
        cell = value;
    }

    void envelopingSpace(Volume value)
    {
        vol = value;
    }

private:
    Volume vol;
    Volume cell;
    std::array<idx_t, ELEMENT_TYPE_COUNT> elems;
    std::array<idx_t, CellInfo::CELL_TYPE_COUNT> tps;
    std::vector<idx_t> clrs;
};

} // namespace basecfd

#endif //BASECFD_GRID_INFO_GUARD_H

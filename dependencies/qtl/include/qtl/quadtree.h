#ifndef __quadtree_h_
#define __quadtree_h_

#include "qtl/algorithm.h"
#include "qtl/vector.h"

#include <cstddef>

namespace qtl
{
	namespace qinternal
	{
		enum class quad_node_type
		{
			INTERNAL,
			LEAF
		};

		template<typename T>
		struct quad_internal_node;

		template<typename T>
		struct quad_base_node
		{
			quad_node_type type = quad_node_type::INTERNAL;
			quad_internal_node<T>* parent = nullptr;
			virtual ~quad_base_node() = default;
		};

		template<typename T>
		struct quad_internal_node : public quad_base_node<T>
		{
			quad_base_node<T>* nw = nullptr;
			quad_base_node<T>* ne = nullptr;
			quad_base_node<T>* sw = nullptr;
			quad_base_node<T>* se = nullptr;

			quad_internal_node() noexcept;
			~quad_internal_node();
		};

		template<typename T>
		struct quad_leaf_node : public quad_base_node<T>
		{
			qtl::vector<T> values;
			quad_leaf_node() noexcept;
			~quad_leaf_node() = default;
		};

		template<typename T>
		inline quad_internal_node<T>::quad_internal_node() noexcept
		{
			this->type = quad_node_type::INTERNAL;
		}

		template<typename T>
		inline quad_internal_node<T>::~quad_internal_node()
		{
			if (nw) delete nw;
			if (ne) delete ne;
			if (sw) delete sw;
			if (se) delete se;
			nw = ne = sw = se = nullptr;
		}

		template<typename T>
		inline quad_leaf_node<T>::quad_leaf_node() noexcept
		{
			this->type = quad_node_type::LEAF;
		}
	}

	enum class Direction : std::size_t
	{
		NORTH_WEST,
		NORTH_EAST,
		SOUTH_WEST,
		SOUTH_EAST
	};

	inline constexpr std::size_t Direction_NW = static_cast<std::size_t>(Direction::NORTH_WEST);
	inline constexpr std::size_t Direction_NE = static_cast<std::size_t>(Direction::NORTH_EAST);
	inline constexpr std::size_t Direction_SW = static_cast<std::size_t>(Direction::SOUTH_WEST);
	inline constexpr std::size_t Direction_SE = static_cast<std::size_t>(Direction::SOUTH_EAST);

	template<typename T, typename Comparator, unsigned short BucketSize = 4>
	class pr_quadtree
	{
	public:
		pr_quadtree(const double x, const double y, const double halfWidth, const double halfHeight);
		pr_quadtree(const pr_quadtree&) = delete;
		pr_quadtree(pr_quadtree&& other) noexcept;
		~pr_quadtree();

		bool contains(const T& value) const;
		void insert(const T& value);
	private:
		Comparator __comparator;
		qinternal::quad_base_node<T>* __root;
		const double __x1, __y1, __x2, __y2;
		
		qinternal::quad_base_node<T>* __recursive_insertion(const T& value, qinternal::quad_base_node<T>* node, const double x1, const double y1, const double x2, const double y2);
		bool __recursive_contains(const T & value, qinternal::quad_base_node<T>* node, const double x1, const double y1, const double x2, const double y2) const;

		static bool __is_inside(const double probeX, const double probeY, const double x1, const double y1, const double x2, const double y2);
	};

	template<typename T, typename Comparator, unsigned short BucketSize>
	inline pr_quadtree<T, Comparator, BucketSize>::pr_quadtree(const double x, const double y, const double halfWidth, const double halfHeight)
		: __x1(x - halfWidth), __y1(y - halfHeight), __x2(x + halfWidth), __y2(y + halfHeight)
	{
		__root = new qinternal::quad_leaf_node<T>;
	}

	template<typename T, typename Comparator, unsigned short BucketSize>
	inline pr_quadtree<T, Comparator, BucketSize>::pr_quadtree(pr_quadtree && other) noexcept
		: __root(other.__root), __x1(other.__x1), __x2(other.__x2), __y1(other.__y1), __y2(other.__y2)
	{
		other.__root = nullptr;
	}

	template<typename T, typename Comparator, unsigned short BucketSize>
	inline pr_quadtree<T, Comparator, BucketSize>::~pr_quadtree()
	{
		delete __root;
	}

	template<typename T, typename Comparator, unsigned short BucketSize>
	inline bool pr_quadtree<T, Comparator, BucketSize>::contains(const T & value) const
	{
		return __recursive_contains(value, __root, __x1, __y1, __x2, __y2);
	}

	template<typename T, typename Comparator, unsigned short BucketSize>
	inline void pr_quadtree<T, Comparator, BucketSize>::insert(const T & value)
	{
		__root = __recursive_insertion(value, __root, __x1, __y1, __x2, __y2);
	}

	template<typename T, typename Comparator, unsigned short BucketSize>
	inline qinternal::quad_base_node<T>* pr_quadtree<T, Comparator, BucketSize>::__recursive_insertion(const T & value, qinternal::quad_base_node<T>* node, const double x1, const double y1, const double x2, const double y2)
	{
		using qinternal::quad_base_node;
		using qinternal::quad_internal_node;
		using qinternal::quad_leaf_node;

		if (node == nullptr) // node doesn't exist
		{
			node = new quad_leaf_node<T>;
			static_cast<quad_leaf_node<T>*>(node)->values.push_back(value);
		}
		else if (node->type == qinternal::quad_node_type::INTERNAL) // internal node
		{
			const double midX = (x2 + x1) / 2.0;
			const double midY = (y2 + y1) / 2.0;

			// (x1, y2) ---- (midX, y2) ---- (x2, y2)
			//     |              |             |
			//     |              |             |
			// (x1, midY) -- (midX, midY) -- (x2, midY)
			//     |              |             |
			//     |              |             |
			// (x1, y1) ---- (midX, y1) ---- (x2, y1)
	
			auto dir = __comparator.direction(midX, midY, value);
			auto this_node = static_cast<qinternal::quad_internal_node<T>*>(node);
			switch (dir)
			{
			case Direction::NORTH_WEST:
			{
				this_node->nw = __recursive_insertion(value, this_node->nw, x1, midY, midX, y2);
				this_node->nw->parent = this_node;
				break;
			}
			case Direction::NORTH_EAST:
			{
				this_node->ne = __recursive_insertion(value, this_node->ne, midX, midY, x2, y2);
				this_node->ne->parent = this_node;
				break;
			}
			case Direction::SOUTH_WEST:
			{
				this_node->sw = __recursive_insertion(value, this_node->sw, x1, y1, midX, midY);
				this_node->sw->parent = this_node;
				break;
			}
			case Direction::SOUTH_EAST:
			{
				this_node->se = __recursive_insertion(value, this_node->se, midX, y1, x2, midY);
				this_node->se->parent = this_node;
				break;
			}
			}
		}
		else // leaf node
		{
			qinternal::quad_leaf_node<T>* this_node = reinterpret_cast<qinternal::quad_leaf_node<T>*>(node);
			if (this_node->values.size() < BucketSize)
			{
				this_node->values.push_back(value);
			}
			else
			{
				vector<T> data = qtl::move(this_node->values);
				quad_internal_node<T>* parent = this_node->parent;
				if (parent)
				{
					Direction dir = Direction::NORTH_WEST;
					if (parent->nw == node) dir = Direction::NORTH_WEST;
					else if (parent->ne == node) dir = Direction::NORTH_EAST;
					else if (parent->sw == node) dir = Direction::SOUTH_WEST;
					else dir = Direction::SOUTH_EAST;
					delete node;
					node = new quad_internal_node<T>;
					switch (dir)
					{
					case Direction::NORTH_WEST:
					{
						parent->nw = node;
						break;
					}
					case Direction::NORTH_EAST:
					{
						parent->ne = node;
						break;
					}
					case Direction::SOUTH_WEST:
					{
						parent->sw = node;
						break;
					}
					case Direction::SOUTH_EAST:
					{
						parent->se = node;
						break;
					}
					}
				}
				else // root
				{
					delete node;
					node = new quad_internal_node<T>;
				}
				for (auto val : data)
				{
					node = __recursive_insertion(val, node, __x1, __y1, __x2, __y2);
				}
				node = __recursive_insertion(value, node, __x1, __y1, __x2, __y2);
			}
		}

		return node;
	}

	template<typename T, typename Comparator, unsigned short BucketSize>
	inline bool pr_quadtree<T, Comparator, BucketSize>::__recursive_contains(const T & value, qinternal::quad_base_node<T>* node, const double x1, const double y1, const double x2, const double y2) const
	{
		using qinternal::quad_base_node;
		using qinternal::quad_internal_node;
		using qinternal::quad_leaf_node;

		if (node == nullptr) // node doesn't exist
		{
			return false;
		}
		else if (node->type == qinternal::quad_node_type::INTERNAL) // internal node
		{
			const double midX = (x2 + x1) / 2.0;
			const double midY = (y2 + y1) / 2.0;

			// (x1, y2) ---- (midX, y2) ---- (x2, y2)
			//     |              |             |
			//     |              |             |
			// (x1, midY) -- (midX, midY) -- (x2, midY)
			//     |              |             |
			//     |              |             |
			// (x1, y1) ---- (midX, y1) ---- (x2, y1)

			auto dir = __comparator.direction(midX, midY, value);
			auto this_node = static_cast<qinternal::quad_internal_node<T>*>(node);
			switch (dir)
			{
			case Direction::NORTH_WEST:
			{
				return __recursive_contains(value, this_node->nw, x1, midY, midX, y2);
			}
			case Direction::NORTH_EAST:
			{
				return __recursive_contains(value, this_node->ne, midX, midY, x2, y2);
			}
			case Direction::SOUTH_WEST:
			{
				return __recursive_contains(value, this_node->sw, x1, y1, midX, midY);
			}
			case Direction::SOUTH_EAST:
			{
				return __recursive_contains(value, this_node->se, midX, y1, x2, midY);
			}
			}
		}
		else // leaf node
		{
			qinternal::quad_leaf_node<T>* this_node = reinterpret_cast<qinternal::quad_leaf_node<T>*>(node);
			return qtl::find(this_node->values.begin(), this_node->values.end(), value) != this_node->values.end();
		}
	}

	template<typename T, typename Comparator, unsigned short BucketSize>
	inline bool pr_quadtree<T, Comparator, BucketSize>::__is_inside(const double probeX, const double probeY, const double x1, const double y1, const double x2, const double y2)
	{
		return x1 <= probeX && probeX <= x2 && y1 <= probeY && probeY <= y2;
	}
}

#endif
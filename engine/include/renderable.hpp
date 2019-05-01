#pragma once

#include "wsdl2/video.hpp"

#include <memory>
#include <set>

namespace flat
{
	/// Literally anything that needs to be rendered
	struct renderable
	{
	public:
		/* The Z value indicates the rendering order.
		 * A Z value of 0 is just after the background.
		 * the higher the value the more on the front (rendered on top)
		 */
		unsigned z = 0;
		bool visible = true;

		virtual ~renderable() {}
		virtual void render() = 0;

		void hide() { visible = false; }
		void show() { visible = true; }
		bool is_visible() { return visible; }
	};

	/* A group of renderable objects sorted by their z value */
	class rendergroup : public renderable, public std::set<std::shared_ptr<renderable>>
	{
	public:
		void render() override {
			for (auto&& child : *this)
				child->render();
		}
	};
}

inline bool operator<(const flat::renderable& lhs, const flat::renderable& rhs)
{
	return lhs.z < rhs.z;
}
#include <iostream>
#include <spine-sdl2/Skeleton.h>
#include <spine/SkeletonData.h>
#include <spine/Slot.h>
#include <spine/Attachment.h>

namespace spine {

Skeleton::Skeleton (SkeletonData *skeletonData) :
				BaseSkeleton(skeletonData),
				vertexArray(skeletonData->bones.size() * 4),
				texture(NULL) {


}

void Skeleton::draw (SDL_Renderer *renderer) const {
	const_cast<Skeleton*>(this)->vertexArray.clear();
	for (int i = 0, n = slots.size(); i < n; i++)
		if (slots[i]->attachment) slots[i]->attachment->draw(slots[i]);

	SDL_RenderGeometry(renderer, texture, (SDL_Vertex*)&vertexArray[0], vertexArray.size(), NULL, 0, NULL);
}

} /* namespace spine */

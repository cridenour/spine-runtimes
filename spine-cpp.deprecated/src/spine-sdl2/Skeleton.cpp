#include <iostream>
#include <spine-sdl2/Skeleton.h>
#include <spine/SkeletonData.h>
#include <spine/Slot.h>
#include <spine/Attachment.h>

namespace spine {

Skeleton::Skeleton (SkeletonData *skeletonData) :
				BaseSkeleton(skeletonData),
				vertexArray(skeletonData->bones.size() * 4),
				indices(skeletonData->bones.size()*6),
				texture(NULL) {


}

void Skeleton::draw (SDL_Renderer *renderer) const {
	const_cast<Skeleton*>(this)->vertexArray.clear();
	const_cast<Skeleton*>(this)->indices.clear();
	for (int i = 0, n = slots.size(); i < n; i++)
		if (slots[i]->attachment) slots[i]->attachment->draw(slots[i]);


/*	for (int i=0; i<vertexArray.size(); i++) {
	    SDL_Vertex *v = (SDL_Vertex*)&vertexArray[i];
	    printf("Vertex %d, at %fx%f texcoord %fx%f color %d,%d,%d,%d\n", i, v->position.x, v->position.y, v->tex_coord.x, v->tex_coord.y, v->color.r, v->color.g, v->color.b, v->color.a);
	}
	//printf("skeleton::draw %u %u\n", vertexArray.size(), texture);*/

	if (vertexArray.size()) {
	    SDL_RenderGeometry(renderer, texture, (SDL_Vertex*)&vertexArray[0], vertexArray.size(), (int*)&indices[0], indices.size(), NULL);
	}
}

} /* namespace spine */

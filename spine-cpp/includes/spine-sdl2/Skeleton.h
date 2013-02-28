#ifndef SPINE_SKELETON_H_
#define SPINE_SKELETON_H_

#include <spine/BaseSkeleton.h>
#include <SDL.h>
#include <spine-sdl2/util.h>

namespace spine {

class Skeleton: public BaseSkeleton {

public:
	VertexArray vertexArray;
	SDL_Texture *texture;

	Skeleton (SkeletonData *skeletonData);

	virtual void draw (SDL_Renderer *renderer) const;
};

} /* namespace spine */
#endif /* SPINE_SKELETON_H_ */

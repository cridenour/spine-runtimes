#ifndef SPINE_REGIONATTACHMENT_H_
#define SPINE_REGIONATTACHMENT_H_

#include <spine/BaseRegionAttachment.h>
#include <SDL.h>
#include <spine-sdl2/util.h>

namespace spine {

class Bone;
class AtlasRegion;

class RegionAttachment: public BaseRegionAttachment {
public:
	SDL_Vertex vertices[4];
	SDL_Texture *texture;

	RegionAttachment (AtlasRegion *region);

	virtual void updateWorldVertices (Bone *bone);
	virtual void draw (Slot *slot);
};

} /* namespace spine */
#endif /* SPINE_REGIONATTACHMENT_H_ */

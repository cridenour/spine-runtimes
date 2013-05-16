#ifndef SPINE_ATLAS_H_
#define SPINE_ATLAS_H_

#include <spine/BaseAtlas.h>
#include <SDL.h>

namespace spine {

class AtlasPage: public BaseAtlasPage {
public:
	SDL_Texture *texture;
	float texw, texh;
	AtlasPage(SDL_Texture *t, float w, float h);
	~AtlasPage();
};

//

class AtlasRegion: public BaseAtlasRegion {
public:
	AtlasPage *page;
};

//

class Atlas: public BaseAtlas {
public:
	Atlas (SDL_Renderer *r, const std::string &text);
	Atlas (SDL_Renderer *r, const char *begin, const char *end);

	AtlasRegion* findRegion (const std::string &name);

private:
    SDL_Renderer *renderer;
	virtual BaseAtlasPage* newAtlasPage (std::string name);
	virtual BaseAtlasRegion* newAtlasRegion (BaseAtlasPage* page);
};

} /* namespace spine */
#endif /* SPINE_ATLAS_H_ */

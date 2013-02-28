#include <SDL.h>
#include <SDL_image.h>
#include <spine-sdl2/Atlas.h>

namespace spine {

Atlas::Atlas (SDL_Renderer *r, const std::string &text):renderer(r) {
	load(text);
}

Atlas::Atlas (SDL_Renderer *r, const char *begin, const char *end):renderer(r) {
	load(begin, end);
}

BaseAtlasPage* Atlas::newAtlasPage (std::string name) {
	AtlasPage *page = new AtlasPage();

	SDL_Surface *surface = IMG_Load(name.c_str());
    if (surface) {
        page->texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (page->texture) {
            SDL_FreeSurface(surface);
            return page;
        }
        SDL_FreeSurface(surface);
        return NULL;
    }

	return NULL;
}

BaseAtlasRegion* Atlas::newAtlasRegion (BaseAtlasPage* page) {
	AtlasRegion *region = new AtlasRegion();
	region->page = reinterpret_cast<AtlasPage*>(page);
	return region;
}

AtlasRegion* Atlas::findRegion (const std::string &name) {
	return reinterpret_cast<AtlasRegion*>(BaseAtlas::findRegion(name));
}

} /* namespace spine */

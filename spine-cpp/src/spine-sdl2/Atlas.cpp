#include <SDL.h>
#include <SDL_image.h>
#include <spine-sdl2/Atlas.h>

namespace spine {

AtlasPage::AtlasPage(SDL_Texture *t, float w, float h): texture(t), texw(w), texh(h)
{
}

Atlas::Atlas (SDL_Renderer *r, const std::string &text):renderer(r) {
	load(text);
}

Atlas::Atlas (SDL_Renderer *r, const char *begin, const char *end):renderer(r) {
	load(begin, end);
}

BaseAtlasPage* Atlas::newAtlasPage (std::string name) {
	SDL_Surface *surface = IMG_Load(name.c_str());
    if (surface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture) {
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            AtlasPage *page = new AtlasPage(texture, (float)surface->w, (float)surface->h);
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

/*******************************************************************************
 * Copyright (c) 2013, Esoteric Software
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include <spine/spine-sdl2.h>
#include <spine/extension.h>

void _AtlasPage_createTexture (AtlasPage* self, const char* path, void *param) {
    self->rendererObject = NULL;

    SDL_Surface *surface = IMG_Load(path);
    if (surface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface((SDL_Renderer*)param, surface);
        if (texture) {
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            self->rendererObject = texture;
            self->width = surface->w;
	        self->height = surface->h;
            SDL_FreeSurface(surface);
            return;
        }
        SDL_FreeSurface(surface);
    }
}

void _AtlasPage_disposeTexture (AtlasPage* self) {
    if (self->rendererObject) {
        SDL_DestroyTexture((SDL_Texture*)self->rendererObject);
        self->rendererObject = NULL;
    }
}

char* _Util_readFile (const char* path, int* length) {
    SDL_RWops *file = SDL_RWFromFile(path, "rb");
    if (!file) return NULL;

    SDL_RWseek(file, 0, SEEK_END);
	*length = SDL_RWtell(file);
	SDL_RWseek(file, 0, SEEK_SET);

	char *data = (char*)malloc(*length);
	SDL_RWread(file, data, 1, *length);
	SDL_RWclose(file);

    return data;
}

/**/

namespace spine {

SkeletonDrawable::SkeletonDrawable (SkeletonData* skeletonData, AnimationStateData* stateData) :
				timeScale(1),
				vertexArray(skeletonData->boneCount * 4),
				indices(skeletonData->boneCount * 6),
				lastUpdate(-1)
{
	Bone_setYDown(true);

	skeleton = Skeleton_create(skeletonData);
	state = AnimationState_create(stateData);
}

SkeletonDrawable::~SkeletonDrawable () {
	AnimationState_dispose(state);
	Skeleton_dispose(skeleton);
}

void SkeletonDrawable::update (Uint64 now) {
    float deltaTime;

    if (lastUpdate != -1) deltaTime = (now - lastUpdate) / 1000.0f;
    else deltaTime = 0.0f;

    lastUpdate = now;
	Skeleton_update(skeleton, deltaTime);
	AnimationState_update(state, deltaTime * timeScale);
	AnimationState_apply(state, skeleton);
	Skeleton_updateWorldTransform(skeleton);
}

void SkeletonDrawable::draw (SDL_Renderer *renderer) {
    // Single texture rendering (no batching).

	float vertexPositions[8];
	int vertexCount = 0;
	SDL_Texture *texture = NULL;
	SDL_Vertex vertices[4];

	vertexArray.clear();
	indices.clear();

	for (int i = 0; i < skeleton->slotCount; ++i) {
		Slot* slot = skeleton->slots[i];
		Attachment* attachment = slot->attachment;
		if (!attachment || attachment->type != ATTACHMENT_REGION) continue;
		RegionAttachment* regionAttachment = (RegionAttachment*)attachment;
		RegionAttachment_computeVertices(regionAttachment, slot, vertexPositions);

		Uint8 r = skeleton->r * slot->r * 255;
		Uint8 g = skeleton->g * slot->g * 255;
		Uint8 b = skeleton->b * slot->b * 255;
		Uint8 a = skeleton->a * slot->a * 255;

		vertices[0].color.r = r;
        vertices[0].color.g = g;
        vertices[0].color.b = b;
        vertices[0].color.a = a;
        vertices[1].color.r = r;
        vertices[1].color.g = g;
        vertices[1].color.b = b;
        vertices[1].color.a = a;
        vertices[2].color.r = r;
        vertices[2].color.g = g;
        vertices[2].color.b = b;
        vertices[2].color.a = a;
        vertices[3].color.r = r;
        vertices[3].color.g = g;
        vertices[3].color.b = b;
        vertices[3].color.a = a;

		vertices[0].position.x = vertexPositions[VERTEX_X1];
		vertices[0].position.y = vertexPositions[VERTEX_Y1];
		vertices[1].position.x = vertexPositions[VERTEX_X2];
		vertices[1].position.y = vertexPositions[VERTEX_Y2];
		vertices[2].position.x = vertexPositions[VERTEX_X3];
		vertices[2].position.y = vertexPositions[VERTEX_Y3];
		vertices[3].position.x = vertexPositions[VERTEX_X4];
		vertices[3].position.y = vertexPositions[VERTEX_Y4];

		texture = (SDL_Texture*)((AtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;

		vertices[0].tex_coord.x = regionAttachment->uvs[VERTEX_X1];
		vertices[0].tex_coord.y = regionAttachment->uvs[VERTEX_Y1];
		vertices[1].tex_coord.x = regionAttachment->uvs[VERTEX_X2];
		vertices[1].tex_coord.y = regionAttachment->uvs[VERTEX_Y2];
		vertices[2].tex_coord.x = regionAttachment->uvs[VERTEX_X3];
		vertices[2].tex_coord.y = regionAttachment->uvs[VERTEX_Y3];
		vertices[3].tex_coord.x = regionAttachment->uvs[VERTEX_X4];
		vertices[3].tex_coord.y = regionAttachment->uvs[VERTEX_Y4];

        indices.push_back(vertexCount);
        indices.push_back(vertexCount+1);
        indices.push_back(vertexCount+2);
        indices.push_back(vertexCount);
        indices.push_back(vertexCount+2);
        indices.push_back(vertexCount+3);
        vertexCount+=4;

		vertexArray.push_back(vertices[0]);
        vertexArray.push_back(vertices[1]);
        vertexArray.push_back(vertices[2]);
        vertexArray.push_back(vertices[3]);
	}

	/*for (int i=0; i<vertexArray.size(); i++) {
	    SDL_Vertex *v = (SDL_Vertex*)&vertexArray[i];
	    printf("Vertex %d, at %fx%f texcoord %fx%f color %d,%d,%d,%d\n", i, v->position.x, v->position.y, v->tex_coord.x, v->tex_coord.y, v->color.r, v->color.g, v->color.b, v->color.a);
	}
	printf("skeleton::draw %lu %u\n", vertexArray.size(), texture);*/

	if (texture) {
	    SDL_RenderGeometry(renderer, texture, (SDL_Vertex*)&vertexArray[0], vertexArray.size(), (int*)&indices[0], indices.size(), NULL);
	}
}

} /* namespace spine */

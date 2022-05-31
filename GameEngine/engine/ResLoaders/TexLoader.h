#ifndef TEX_LOADER__H
#define TEX_LOADER__H

//container and executer for objects

struct Texture;
struct WinTexture;

class TextureLoader
{
private:
	TextureLoader() {};
public:

	static Texture* LoadTexture(const char*);

	static WinTexture* LoadWinTexture(const char*);
};

#endif // !ENGINE__H

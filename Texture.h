#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <fstream>

class Texture
{
private:

	ID3D11ShaderResourceView* texture;
	int width;
	int height;

public:

	Texture(std::string filename, ID3D11Device* device);
	~Texture();
	ID3D11ShaderResourceView* GetTexture() const;

};


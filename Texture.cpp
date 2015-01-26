#include "Texture.h"

using namespace std;
using namespace DirectX;

Texture::Texture(std::string filename, ID3D11Device* device)
{
	ifstream file(filename, ios_base::binary);
	if (file.good())
	{
		vector<unsigned char> data;

		//Get the filesize in bytes
		file.seekg(0, file.end);
		int size = file.tellg();
		file.seekg(0, file.beg);

		data.resize(size);

		//Assume texture is always square and RGBA format
		width = height = sqrt(size / 4);
		
		//Read the whole file into the vector
		file.read((char*)&data[0], data.size());

		file.close();

		//Create texture buffer
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.MiscFlags = 0;
		textureDesc.CPUAccessFlags = 0;

		ID3D11Texture2D* pTexture = nullptr;
		D3D11_SUBRESOURCE_DATA subresourceData;
		ZeroMemory(&data, sizeof(data));

		subresourceData.pSysMem = (void*)&data[0];
		subresourceData.SysMemPitch = width * 4 * sizeof(char);

		device->CreateTexture2D(&textureDesc, &subresourceData, &pTexture);

		D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc;
		ZeroMemory(&resViewDesc, sizeof(resViewDesc));
		resViewDesc.Format = textureDesc.Format;
		resViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		resViewDesc.Texture2D.MostDetailedMip = 0;
		device->CreateShaderResourceView(pTexture, &resViewDesc, &texture);

		pTexture->Release();
	}
	else
	{
		texture = nullptr;
	}
}


Texture::~Texture()
{
	texture->Release();
}

ID3D11ShaderResourceView* Texture::GetTexture() const
{
	return texture;
}


#pragma once

#ifndef SHADER_SYSTEM_H
#define SHADER_SYSTEM_H

//#include "map_vector.h"
#include "DelegatesSystem.h"


/*
#include <d3dcompiler.h>

#include "modules\Facilities_LIB\Facilities_LIB.h"

#include "modules\DDSTextureLoader\DDSTextureLoader.h"

//#include "query.h"
#include "modules\dxerr\dxerr.h" // DXTrace()

*/

//#include <iostream>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>
#include <d3d11.h>

/*
#pragma comment (lib, "modules\\DDSTextureLoader\\DDSTextureLoader.lib")
#pragma comment (lib, "modules\\dxerr\\dxerr.lib")
#pragma comment (lib, "modules\\Facilities_LIB\\Facilities.lib") // string_facilities::pchar_to_pwchar(char* str)
#pragma comment (lib, "d3dcompiler.lib") // for employement of D3DCompileFromFile wich will be accessible after unplugging legacy SDK, for now D3DX11CompileFromFile()
*/



namespace effects
{

namespace shader_system
{



//typedef unsigned int uint;


class Shader;


/*
template<class Key, class Val>
class map_vector: public std::vector<Val>
{
public:

	Val& operator[](const Key& idx);
	Val& operator[](const uint& idx);

	void SetKeys(std::vector<Key>* keys); // сохранить указатель на вектор ключей, чтобы избежать дублирования данных
	//void SetKeys(std::vector<Key>::iterator keys);


private:

	uint index_of(const std::vector<Key>& key_vector, const Key& search_val);

	std::vector<Key>* keys; // храним указатель на вектор ключей, чтобы избежать дублирования данных

};
*/

class ShaderSystem;




class Effects
{
public:

	Effects();

	void Init(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext);

	void Add(const std::string& name); // используется когда у всех эффектов одни и те же d3dDevice и d3dDeviceContext
	void Add(const std::string& name, ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext); // если d3dDevice и d3dDeviceContext
																											 // у каждого эффекта свои

	void Add(const std::string& name, const ShaderSystem& effect); // если нужно добавить готовый эффект
	void Add(const std::string& name, const ShaderSystem& effect, 
		               ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext); // если нужно добавить готовый эффект с новыми
																						// d3dDevice и d3dDeviceContext

	uint size(); // количество эффектов в коллекции

	



	ShaderSystem& operator[](const std::string& key);
	ShaderSystem& operator[](const uint& idx);
	

private:

  typedef map_vector<std::string, ShaderSystem> EffectsCollection;

	EffectsCollection effects;	
	


	std::vector<std::string> names;




	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;

};



enum ShaderTypes{VertexShader, HullShader, DomainShader, GeometryShader, PixelShader, ComputeShader};




class ShaderSystem
{

public:

	ShaderSystem();	
	//{
	//	shaders.SetKeys(&shader_names); // map_vector<>::SetKeys() - передаём указатель на вектор имён шейдеров, вместо копирования;
										// далее контейнер map_vector будет использовать имена шейдеров для доступа к шейдерам по ключу
	//}

	ShaderSystem(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext);
	

	void Init(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext); // использовать этот метод, когда объект будет применять эффекты, поставляемые ShaderSystem
	

	void AddShader(std::string shader_name,				   
				   const ShaderTypes& ShaderType,
				   char* szFileName, 
				   LPCSTR szEntryPoint, 
				   LPCSTR szShaderModel);

	void set();



	void begin();
	void end();


	void AddBeginFunction(delegates_system::Delegate f, std::string const & name);
	void AddEndFunction(delegates_system::Delegate f, std::string const & name);


	delegates_system::DelegatesSystem begin_functions;
	delegates_system::DelegatesSystem end_functions;


	typedef map_vector<std::string, Shader> ShadersCollection;
	
	ShadersCollection shaders; // коллекция шейдеров


	  ID3DBlob* get_VS_byte_code();

	  void set_device(ID3D11Device* d3dDevice);
	  void set_device_context(ID3D11DeviceContext* d3dDeviceContext);


private:

	std::vector<std::string> shader_names;	
	
	ID3DBlob* VS_byte_code;	

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;	

};

class IConstantBuffer;

template<class T> class ConstantBuffer;





	class ISetter
	{
	public:

		virtual void set_shader() = 0;

		virtual void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers) = 0;

		virtual void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV) = 0;

	protected:

		ID3D11DeviceContext* d3dDeviceContext;

	};


	class VertexShaderSetter: public ISetter
	{
	public:
		
		VertexShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11VertexShader* VS);

		void set_shader();

		void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers);

		void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV);
	
	private:

		ID3D11VertexShader* VS;

	};


	class HullShaderSetter: public ISetter
	{
	public:

		HullShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11HullShader* HS);
	
		void set_shader();

		void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers);

		void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV);
	
	private:

		ID3D11HullShader* HS;

	};


	class DomainShaderSetter: public ISetter
	{
	public:

		DomainShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11DomainShader* DS);
		
		void set_shader();

		void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers);

		void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV);
	
	private:

		ID3D11DomainShader* DS;

	};


	class GeometryShaderSetter: public ISetter
	{
	public:

		GeometryShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11GeometryShader* GS);

		void set_shader();
		
		void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers);

		void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV);
		
	private:

		ID3D11GeometryShader* GS;

	};
	

	class PixelShaderSetter: public ISetter
	{
	public:

		PixelShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11PixelShader* PS);

		void set_shader();
		
		void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers);

		void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV);
		
	private:

		ID3D11PixelShader* PS;

	};


	class ComputeShaderSetter: public ISetter
	{
	public:
		
		ComputeShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11ComputeShader* CS);

		void set_shader();
		
		void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers);

		void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV);
		
	
	private:

		ID3D11ComputeShader* CS;

	};



class Texture2D;


class Shader
{

public:

	Shader()
	{
	
	};


	Shader(std::string shader_name,
		   ID3D11Device* d3dDevice,	
		   ID3D11DeviceContext* d3dDeviceContext, 
		   const ShaderTypes& ShaderType,
		   WCHAR* szFileName, 
		   LPCSTR szEntryPoint, 
		   LPCSTR szShaderModel);


	Shader(const Shader& rhs);

	
	std::string& get_name();

	template<class T> void AddConstantBuffer(const std::string& buffer_name,
										     const uint& StartSlot,
											 const T* content = NULL,
										     const uint& NumBuffers = 1,
										     D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC, 
										     UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE, 
										     UINT MiscFlags = 0);

	
	void AddTexture2D(char* file_name, const uint& StartSlot, const uint& NumViews = 1);

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);

	void create(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel); // происходит компиляция шейдера и выбор установщика на основе 
																			   // типа шейдера
	//void create(

	void set();

	void set_Texture2D(ID3D11ShaderResourceView* const * SRV, const uint& StartSlot, const uint& NumViews);



	typedef map_vector<std::string, IConstantBuffer*> ConstantBuffersCollection; // интерфейс IConstantBuffer нужен для того, чтобы можно было
																		    // создать коллекцию КБ, не указывая параметр шаблона ConstantBuffer<T>,
																			// т.к. мы не знаем заранее какой будет тип содержимого КБ
	
		 ConstantBuffersCollection constant_buffers; // коллекция КБ


	typedef map_vector<std::string, Texture2D> Textures2DCollection;

	   Textures2DCollection textures2D;





	/*
	ID3D11DeviceContext** get_device_context()
	{
		return &d3dDeviceContext;
	}
	*/

	ID3D11DeviceContext* get_device_context();

	ID3DBlob* get_byte_code();

	ID3D11VertexShader* GetVS();

	ID3D11HullShader* GetHS();

	ID3D11DomainShader* GetDS();

	ID3D11GeometryShader* GetGS();

	ID3D11PixelShader* GetPS();

	ID3D11ComputeShader* GetCS();
	
//protected:
private:

	std::string name;

	std::vector<std::string> constant_buffers_names;

	std::vector<std::string> textures2D_names;


	ShaderTypes ShaderType;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;


	ID3DBlob* pShaderCompilationBlob; // будет содержать шейдерный байт-код после компиляции шейдера


	// каким шейдером может стать:
	ID3D11VertexShader* vs;
	ID3D11HullShader* hs;
	ID3D11DomainShader* ds;
	ID3D11GeometryShader* gs;
	ID3D11PixelShader* ps;	
	ID3D11ComputeShader* cs;	


	ISetter* setter; // поле для хранения полиморфного объекта, тип которого будет определён во время выполнения метода create(...)	

};


class Specification
{
public:

	Specification(std::string FX_name, std::string shader_name, uint StartSlot, uint NumViews = 1);
	

public:

	std::string FX_name;
	std::string shader_name;
	
	uint StartSlot;
	uint NumViews;

};



class Texture2D
{

public:

	Texture2D() = default;

	Texture2D(char* file_name, const uint& StartSlot, const uint& NumViews, ID3D11Device* d3dDevice, ISetter* setter);

	Texture2D(ID3D11ShaderResourceView* SRV, const uint& StartSlot, const uint& NumViews, ID3D11Device* d3dDevice, ISetter* setter);

	Texture2D(char* file_name, const std::vector<Specification>& specifications);
	Texture2D(ID3D11ShaderResourceView* SRV, const std::vector<Specification>& specifications);

	Texture2D(char* file_name, Specification const & specification);
	Texture2D(ID3D11ShaderResourceView* SRV, Specification const & specification);

	void set();

	void apply();



private:

ID3D11ShaderResourceView* SRV;

effects::shader_system::Effects* FX_Group;

std::vector<Specification> specifications;

delegates_system::DelegatesSystem delegates;


char* file_name;

uint StartSlot;
uint NumViews;


ID3D11Device* d3dDevice;

ISetter* setter;



};


/*
class Textures2DSystem
{
public:

	void add(char* file_name, Specification const & specification);
	void add(ID3D11ShaderResourceView* SRV, Specification const & specification);



private:

	map_vector<std::string, Texture2D> textures;

};
*/

class IConstantBuffer
{

public:

	virtual std::string get_name() const = 0;
	virtual void set() = 0;
	virtual void* operator=(void* cb) = 0;
	virtual ID3D11Buffer* GetD3DBuffer() const = 0;
};


template<class T>
class ConstantBuffer: public IConstantBuffer
{

public:

	ConstantBuffer(const std::string& name, 
				   const uint& StartSlot, 
				   const T* content,
				   const uint& NumBuffers, 
				   ID3D11Device* d3dDevice,
				   ID3D11DeviceContext* d3dDeviceContext, 
				   ISetter* setter,  
				   D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC, 
				   UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE, 
				   UINT MiscFlags = 0);



	virtual std::string get_name() const; // перегрузка метода IConstantBuffer
	



	virtual void* operator=(void* cb); // перегрузка метода IConstantBuffer
	


	void create(D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC, UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE, UINT MiscFlags = 0);	


	void update();


	void set();  // перегрузка метода IConstantBuffer
	


	T& get_content();

	void set_content(const T& rhs);


	    __declspec(property(get = get_content, put = set_content)) T Content;



	ID3D11Buffer* GetD3DBuffer() const; // перегрузка метода IConstantBuffer
	
	
private:

	std::string name;

	typedef typename T type;

	T content;

	bool isSet;

	ISetter* setter;

	uint StartSlot; // индекс регистра cbuffer в HLSL (b#, cb#)
	uint NumBuffers;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;

	ID3D11Buffer* d3dConstantBuffer;

};



template<class T> T& access(IConstantBuffer* buf);


} // namespace shader_system

} // namespace effects


#endif





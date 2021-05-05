#pragma once
#include <memory>
#include <vector>


class InputMemoryStream
{
private:
	char* mBuffer;
	size_t mHead;
	size_t mCapacity;
public:

	InputMemoryStream(char* _inBuffer, size_t _inByteCount) : mCapacity(_inByteCount), mHead(0)
	{
		mBuffer = _inBuffer;
	}

	~InputMemoryStream()
	{

	}

	char* GetBuffer() 
	{
		return mBuffer;
	}

	void Read(void* _outData, size_t _inByteCount)
	{
		//Después de haber leído, la posición del siguiente dato a leer quedará en resultHead
		size_t resultHead = mHead + _inByteCount;

		//Si resultHead supera la capacidad del mBuffer, estamos leyendo más de lo que escribimos
		//Algo hemos programado mal por ahí fuera: Revisemos
		if (resultHead > mCapacity)
		{
			throw std::exception("InputMemoryStream::No data to read");
		}
		//Copio en _outData el tamaño de _outData contado desde mHead
		std::memcpy(_outData, mBuffer + mHead, _inByteCount);

		//Avanzo mHead para el siguiente Read.
		mHead = resultHead;
	}

	template<typename T> void Read(T* _outData)
	{
		//Tal y cómo está hecho, este Read sólo funciona para tipos básicos.
		//Así evitamos deserializar algo que no se hará bien
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Este Read solo soporta tipos basicos.");
		//En sizeOf se guarda el tamaño de este tipo en bytes
		//Si es un char: 1
		//Si es un int: 4
		int sizeOf = sizeof(*_outData);
		Read(_outData, sizeOf);
	}

	void Read(std::string* _outData)
	{

	}

};


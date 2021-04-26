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

	void Read(void* _outData, size_t _inByteCount)
	{
		//Despu�s de haber le�do, la posici�n del siguiente dato a leer quedar� en resultHead
		size_t resultHead = mHead + _inByteCount;

		//Si resultHead supera la capacidad del mBuffer, estamos leyendo m�s de lo que escribimos
		//Algo hemos programado mal por ah� fuera: Revisemos
		if (resultHead > mCapacity)
		{
			throw std::exception("InputMemoryStream::No data to read");
		}
		//Copio en _outData el tama�o de _outData contado desde mHead
		std::memcpy(_outData, mBuffer + mHead, _inByteCount);

		//Avanzo mHead para el siguiente Read.
		mHead = resultHead;
	}

	template<typename T> void Read(T* _outData)
	{
		//Tal y c�mo est� hecho, este Read s�lo funciona para tipos b�sicos.
		//As� evitamos deserializar algo que no se har� bien
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Este Read solo soporta tipos basicos.");
		//En sizeOf se guarda el tama�o de este tipo en bytes
		//Si es un char: 1
		//Si es un int: 4
		int sizeOf = sizeof(*_outData);
		Read(_outData, sizeOf);
	}


};


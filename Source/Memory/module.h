#pragma once

class Module
{
public:

	Module( DWORD_PTR ImageBase, DWORD_PTR ImageSize, const std::string& ImageName, const std::string& ImagePath )
		:	
		m_ImageBase( ImageBase ),
		m_ImageSize( ImageSize ),
		m_ImageName( ImageName ),
		m_ImagePath( ImagePath )
	{
	}

	DWORD_PTR GetImage() const
	{
		if (m_ImageBase)
			return m_ImageBase;
		else
			return 0;
	}

	DWORD_PTR GetSize() const
	{
		return m_ImageSize;
	}

	const std::string&	GetName() const
	{
		return m_ImageName;
	}

	const std::string&	GetPath() const
	{
		return m_ImagePath;
	}

private:

	DWORD_PTR			m_ImageBase = 0;
	DWORD_PTR			m_ImageSize = 0;
	std::string			m_ImageName;
	std::string			m_ImagePath;
};



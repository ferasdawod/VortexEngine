#pragma once


#define DECLARE_PROPERTY(varType, varName) \
	protected: \
	varType _ ## varName; \
	public: \
	varType	Get ## varName() const { return _ ## varName; } \
	void	Set ## varName(const varType& val) { _ ## varName = val; }

#define DECLARE_PROPERTY_READ_ONLY(varType, varName) \
	protected: \
	varType _ ## varName; \
	public: \
	varType Get ## varName() const { return _ ## varName; }

#define DECLARE_STRING_PROPERTY(varName) \
	protected: \
	std::string _ ## varName; \
	public: \
	const std::string&	Get ## varName() const { return _ ## varName; } \
	void				Set ## varName(const std::string& val) { _ ## varName = val; }

#define DECLARE_STRING_PROPERTY_READ_ONLY(name) \
	protected: \
	std::string _ ## name; \
	public: \
	const std::string&	Get ## name() const { return _ ## name; } \


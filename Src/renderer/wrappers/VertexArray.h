#pragma once

class VertexArray
{
public:
	enum class Type { NONE, GUI, WORLD, SKYBOX };

	VertexArray();
	VertexArray(VertexArray::Type type);
	VertexArray(VertexArray&& other) noexcept;

	VertexArray& operator=(VertexArray&& other) noexcept;

	~VertexArray();

	void bind() const;
	void setupAttributeData() const;

	static VertexArray CreateForNone();
	static VertexArray CreateForGUI();
	static VertexArray CreateForWorld();
	static VertexArray CreateForSkybox();

private:
	unsigned int ID;
	Type type;

	VertexArray(const VertexArray& other) = delete;
	void operator=(const VertexArray& other) = delete;
};
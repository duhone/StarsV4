#pragma once

class DirectxCheck
{
public:
	DirectxCheck(void);
	virtual ~DirectxCheck(void);
	
	static bool CheckShader11(void);
	static bool CheckShadder20(void);
	static bool checkDirectx9(void);
	static bool checkMedTexture(void);
	static bool checkHighTexture(void);
};

#pragma once

/*
================================================This is no longer used==================================================
*/

enum MatrixType
{
	No_Values,
	Identity,
	Zero,
	ThreeByThree,
};

struct Matrix4
{
	Matrix4(MatrixType Typein);
	Matrix4();
	std::vector<GLfloat> Values;
	void printMatrix()
	{
		for (size_t i = 0; i < 4; i++)
		{
			std::cout << "Line " << i << ": ";
			for (size_t j = 0; j < 4; j++)
			{
				if (i * 4 + j > Values.size() - 1) { break; }
				std::cout << Values.at(i * 4 + j) << ", ";
			}
			std::cout << std::endl;
		}

		for (size_t i = 16; i < Values.size(); i++)
		{
			std::cout << "Left over in array :" << Values.at(i) << std::endl;
		}
	}
	void printMatrix3()
	{
		for (size_t i = 0; i < 3; i++)
		{
			std::cout << "Line " << i << ": ";
			for (size_t j = 0; j < 3; j++)
			{
				if (i * 3 + j > Values.size() - 1) { break; }
				std::cout << Values.at(i * 3 + j) << ", ";
			}
			std::cout << std::endl;
		}

		for (size_t i = 16; i < Values.size(); i++)
		{
			std::cout << "Left over in array :" << Values.at(i) << std::endl;
		}
	}
	Matrix4& operator*=(const Matrix4 B)
	{
		Matrix4 RetM = Matrix4(MatrixType::No_Values);

		for (int Collumn = 0; Collumn < 4; Collumn++)
		{
			for (int Row = 0; Row < 4; Row++)
			{
				float Value = 0.0f;
				for (int Target = 0; Target < 4; Target++)
				{
					Value += this->Values.at(Row * 4 + Target) * B.Values.at(Target * 4 + Collumn);
				}
				RetM.Values.push_back(Value);
			}
		}
		return *this;
	}
};


Matrix4::Matrix4(MatrixType Typein)
{
	switch (Typein)
	{
	case No_Values:
		break;

	case Identity:
		for (size_t i = 0; i < 16; i++)
		{
			Values.push_back(0.0f);
		}
		Values.at(0) = 1.0f;
		Values.at(5) = 1.0f;
		Values.at(10) = 1.0f;
		Values.at(15) = 1.0f;
		break;

	case Zero:
		for (size_t i = 0; i < 16; i++)
		{
			Values.push_back(0.0f);
		}
		break;

	case ThreeByThree:
		for (size_t i = 0; i < 12; i++)
		{
			Values.push_back(0.0f);
		}
		Values.at(0) = 1.0f;
		Values.at(4) = 1.0f;
		Values.at(8) = 1.0f;
		break;

	default:
		for (size_t i = 0; i < 16; i++)
		{
			Values.push_back(0.0f);
		}
		break;
	};

}

Matrix4::Matrix4()
{
	for (size_t i = 0; i < 16; i++)
	{
		Values.push_back(0.0f);
	}
	Values.at(0) = 1.0f;
	Values.at(5) = 1.0f;
	Values.at(10) = 1.0f;
	Values.at(15) = 1.0f;
}

Matrix4 SetProjectionMatrixB(float FOV, float AspectRation, float NP, float FP)//np - near plane, fp - far plane
{
	float Scale = tan(FOV * 0.5f * (3.14 / 180)) * NP;
	float r = AspectRation * Scale; //right 
	float l = -r; //left
	float t = Scale; //top
	float b = -t; //bottom
	//bit of a mess
	Matrix4 RetMat = Matrix4(MatrixType::No_Values);

	//From the tutorial
	/*RetMat.Values.insert(RetMat.Values.begin(),
	{
	2.0f * NP / (r - l),	0.0f, 0.0f, 0.0f,
	0.0f, 2 * NP / (t - b), 0.0f, 0.0f,
	(r + l) / (r - l),(t + b) / (t - b),-(FP + NP) / (FP - NP),-2.0f * (FP * NP) / (FP - NP), //First 2 values are 0, no idea why we need to calculate it.
	0.0f, 0.0f, -1.0f, 0.0f,
	});*/

	RetMat.Values.insert(RetMat.Values.begin(),
		{
		2.0f * NP / (r - l),	0.0f, 0.0f, 0.0f,
		0.0f, 2 * NP / (t - b), 0.0f, 0.0f,
		0.0f,0.0f,-(FP + NP) / (FP - NP),-2.0f * (FP * NP) / (FP - NP),
		0.0f, 0.0f, -1.0f, 0.0f,
		});

	//transpoased projection matrix
	//RetMat.Values.insert(RetMat.Values.begin(),
	//	{
	//	2.0f * NP / (r - l),0.0f, 0.0f, 0.0f,
	//	0.0f, 2 * NP / (t - b),0.0f, 0.0f,
	//	(r + l) / (r - l),(t + b) / (t - b),-(FP + NP) / (FP - NP),-1.0f,
	//	0.0f,0.0f, -2 * FP * NP / (FP - NP), 0.0f,
	//	});

	return RetMat;
};
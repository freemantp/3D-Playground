#pragma once

class Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4(GLfloat mat[4][4]);
	Matrix4x4(GLfloat t00, GLfloat t01, GLfloat t02, GLfloat t03,
            GLfloat t10, GLfloat t11, GLfloat t12, GLfloat t13,
            GLfloat t20, GLfloat t21, GLfloat t22, GLfloat t23,
            GLfloat t30, GLfloat t31, GLfloat t32, GLfloat t33);

	~Matrix4x4();

	bool operator==(const Matrix4x4 &m2) const;
    bool operator!=(const Matrix4x4 &m2) const;
    friend Matrix4x4 Transpose(const Matrix4x4 &);
    void Print(FILE *f) const ;
    static Matrix4x4 Mul(const Matrix4x4 &m1, const Matrix4x4 &m2);
    friend Matrix4x4 Inverse(const Matrix4x4 &);

    GLfloat m[4][4];
};



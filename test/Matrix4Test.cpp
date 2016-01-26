/**
**    Hatchit Engine
**    Copyright(c) 2015 Third-Degree
**
**    GNU Lesser General Public License
**    This file may be used under the terms of the GNU Lesser
**    General Public License version 3 as published by the Free
**    Software Foundation and appearing in the file LICENSE.LGPLv3 included
**    in the packaging of this file. Please review the following information
**    to ensure the GNU Lesser General Public License requirements
**    will be met: https://www.gnu.org/licenses/lgpl.html
**
**/

#include <gtest/gtest.h>
#include "ht_matrix3.h"
#include "ht_matrix4.h"

using namespace Hatchit;
using namespace Math;

TEST(Matrix4, DefaultConstructor)
{
  //Should be an identity matrix
  Matrix4 matrix;

  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 4; j++)
    {
      if(i == j)
        ASSERT_EQ(matrix[i][j], 1);
      else
        ASSERT_EQ(matrix[i][j], 0);
    }
  }
}

TEST(Matrix4, ArrayConstructor)
{
  float array[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

  Matrix4 matrix(array);

  int index = 0;
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 4; j++)
    {
        ASSERT_EQ(matrix[i][j], array[index]);
        index++;
    }
  }
}

TEST(Matrix4, FloatConstructor)
{
  Matrix4 matrix(1,2,3,4,
                 5,6,7,8,
                 9,10,11,12,
                 13,14,15,16);

  int value = 0;
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 4; j++)
    {
        value++;
        ASSERT_EQ(matrix[i][j], value);
    }
  }
}

TEST(Matrix4, Vector3Constructor)
{
  Vector3 row1(1,2,3);
  Vector3 row2(4,5,6);
  Vector3 row3(7,8,9);
  Vector3 row4(10,11,12);

  //Just to easily test in the nested loop
  Vector3 rows[] = {row1, row2, row3, row4};

  /** The resulting matrix fills in a few values so that it will match this
   * layout:
   * 1, 2, 3 0
   * 4, 5, 6, 0
   * 7, 8, 9, 0
   * 10, 11, 12, 1
   */
  Matrix4 matrix(row1, row2, row3, row4);

  for(int i = 0; i < 4; i++)
  {
    Vector3 row = rows[i];
    for(int j = 0; j < 4; j++)
    {
      //Special cases when testing the last column
      if(j == 3 && i == 3)
        ASSERT_EQ(matrix[i][j], 1);
      else if(j == 3)
        ASSERT_EQ(matrix[i][j], 0);
      else
        ASSERT_EQ(matrix[i][j], row[j]);
    }
  }
}

TEST(Matrix4, Vector4Constructor)
{
  Vector4 row1(1,2,3,4);
  Vector4 row2(5,6,7,8);
  Vector4 row3(9,10,11,12);
  Vector4 row4(13,14,15,16);

  //Just to easily test in the nested loop
  Vector4 rows[] = {row1, row2, row3, row4};

  Matrix4 matrix(row1, row2, row3,row4);

  for(int i = 0; i < 4; i++)
  {
    Vector4 row = rows[i];
    for(int j = 0; j < 4; j++)
        ASSERT_EQ(matrix[i][j], row[j]);
  }
}

TEST(Matrix4, Transposition)
{
  Matrix4 matrix(1,2,3,4,
                 4,3,2,1,
                 3,2,4,1,
                 3,1,4,2);
  Matrix4 transpose = matrix.getTranspose();

  //Result taken from wolfram alpha
  //http://www.wolframalpha.com/input/?i=matrix+transpose&a=*C.matrix+transpose-_*Calculator.dflt-&f2={{1%2C2%2C3%2C4}%2C{4%2C3%2C2%2C1}%2C{3%2C2%2C4%2C1}%2C{3%2C1%2C4%2C2}}&f=MatrixOperations.theMatrix_{{1%2C2%2C3%2C4}%2C{4%2C3%2C2%2C1}%2C{3%2C2%2C4%2C1}%2C{3%2C1%2C4%2C2}}
  ASSERT_EQ(transpose[0][0], 1);
  ASSERT_EQ(transpose[0][1], 4);
  ASSERT_EQ(transpose[0][2], 3);
  ASSERT_EQ(transpose[0][3], 3);

  ASSERT_EQ(transpose[1][0], 2);
  ASSERT_EQ(transpose[1][1], 3);
  ASSERT_EQ(transpose[1][2], 2);
  ASSERT_EQ(transpose[1][3], 1);

  ASSERT_EQ(transpose[2][0], 3);
  ASSERT_EQ(transpose[2][1], 2);
  ASSERT_EQ(transpose[2][2], 4);
  ASSERT_EQ(transpose[2][3], 4);

  ASSERT_EQ(transpose[3][0], 4);
  ASSERT_EQ(transpose[3][1], 1);
  ASSERT_EQ(transpose[3][2], 1);
  ASSERT_EQ(transpose[3][3], 2);
}

TEST(Matrix4, Inversion)
{
  Matrix4 matrix(1,2,3,4,
                 4,3,2,1,
                 3,2,4,1,
                 3,1,4,2);
  Matrix4 inversion = matrix.getInverse();

  float* result = (inversion * matrix).getAsArray();
  float* identity = Matrix4().getAsArray();

  // A matrix multiplied by its inverse should be an identity matrix
  for(int i = 0; i < 16; i++)
  {
    // Using ASSERT_NEAR because floating point values aren't exact
    ASSERT_NEAR(result[i], identity[i], 0.000001f);
  }
}

TEST(Matrix4, GetAsArray)
{
    Matrix4 matrix(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    float* array = matrix.getAsArray();

    for(int i = 0; i < 16; i++)
      ASSERT_EQ(array[i], i+1);
}

TEST(Matrix4, Matrix4MultiplicationOperator)
{
  Matrix4 mat1(1,2,3,4,
               4,3,2,1,
               3,2,4,1,
               3,1,4,2);
  Matrix4 mat2(4,5,6,7,
               7,6,5,4,
               6,5,7,4,
               6,4,7,5);

  Matrix4 result = mat1 * mat2;

  ASSERT_EQ(result[0][0], 60);
  ASSERT_EQ(result[0][1], 48);
  ASSERT_EQ(result[0][2], 65);
  ASSERT_EQ(result[0][3], 47);

  ASSERT_EQ(result[1][0], 55);
  ASSERT_EQ(result[1][1], 52);
  ASSERT_EQ(result[1][2], 60);
  ASSERT_EQ(result[1][3], 53);

  ASSERT_EQ(result[2][0], 56);
  ASSERT_EQ(result[2][1], 51);
  ASSERT_EQ(result[2][2], 63);
  ASSERT_EQ(result[2][3], 50);

  ASSERT_EQ(result[3][0], 55);
  ASSERT_EQ(result[3][1], 49);
  ASSERT_EQ(result[3][2], 65);
  ASSERT_EQ(result[3][3], 51);
}

TEST(Matrix4, Vector3MultiplicationOperator)
{
  Matrix4 matrix(1,2,3,4,
                 4,3,2,1,
                 3,2,4,1,
                 3,1,4,2);
  Vector3 vector(1,2,3);

  Vector3 result = matrix * vector;

  ASSERT_EQ(result[0], 18);
  ASSERT_EQ(result[1], 17);
  ASSERT_EQ(result[2], 20);
}

TEST(Matrix4, Vector4MultiplicationOperator)
{
  Matrix4 matrix(1,2,3,4,
                 4,3,2,1,
                 3,2,4,1,
                 3,1,4,2);
  Vector4 vector(1,2,3,4);

  Vector4 result = matrix * vector;

  ASSERT_EQ(result[0], 30);
  ASSERT_EQ(result[1], 20);
  ASSERT_EQ(result[2], 23);
  ASSERT_EQ(result[3], 25);
}

TEST(Matrix4, Matrix3ConversionOperator)
{
  Matrix4 matrix(1,2,3,4,
                 4,5,6,1,
                 7,8,9,1,
                 3,1,4,2);
  Matrix3 result = (Matrix3)matrix;

  int value = 0;
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 3; j++)
    {
      value++;
      ASSERT_EQ(result[i][j], value);
    }
  }
}

TEST(Matrix4Static, OrthographicProjection)
{
  float left = 50;
  float right = -50;
  float bottom = -25;
  float top = 25;
  float _near = 0.1f;
  float _far = 100;

  Matrix4 ortho = Matrix4::GetOrthographicProjection(left, right, bottom, top, _near, _far);

  ASSERT_NEAR(ortho[0][0], 2/(right - left), 0.00001f);
}

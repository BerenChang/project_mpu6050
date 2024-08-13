cmake_minimum_required(VERSION 3.25)
project(project_mpu6050 VERSION 1.0.0)

add_library(
	i2c SHARED
	I2Cdev.h
	I2Cdev.cpp
	)

add_library(
	mpu6050 SHARED
	MPU6050.h
	MPU6050.cpp
	)

add_executable(project_mpu6050 main.cpp I2Cdev.cpp MPU6050.cpp)

add_subdirectory(matplotplusplus)

target_link_libraries(project_mpu6050 PUBLIC matplot)

# target_link_libraries(project_mpu6050 i2c mpu6050)

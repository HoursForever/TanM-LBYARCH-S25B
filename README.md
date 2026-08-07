# TanM-LBYARCH-S25B
The final project 


Grayscale Double-Precision to Integer Image Conversion (C & x86-64 SIMD Assembly)

Overview

This repository implements a high-performance image processing kernel in x86-64 SIMD Assembly linked with a C driver framework. The kernel converts normalized double-precision floating-point grayscale pixel values [0.0, 1.0] into integer grayscale intensity values [0, 255] using packed SIMD instructions.1. Correctness & Sample Verification

The assembly kernel was validated using a sample $3 \times 4$ double-precision matrix scaled by $255.0$ and compared against the C reference implementation:

Input Float Matrix ($3 \times 4$)
0.25  0.35  0.45  0.33
0.55  0.65  0.75  0.33
0.85  0.95  0.15  0.33

Assembly Output Matrix (Converted 0 - 255)
 64   89  115   84
140  166  191   84
217  242   38   84


2. Performance Benchmark Results

<img width="514" height="140" alt="image" src="https://github.com/user-attachments/assets/a8a07947-c350-4557-b41f-4ea7a26d622c" />

Video Demo:
https://drive.google.com/file/d/1JVtEn0OlC8db4o-atELkH3lJb_3ESY1J/view?usp=sharing

The output shows that the program passed in all three cases or image sizes 
Going from 100 pixels to 10,000 pixels (100 times more work) increased time by roughly approximately 112 times.
Going from 10,000 pixels to 1,000,000 pixels (100 times more work) increased time by roughly approximately 190 times.


Short Video Demo





How to run:

Run in powershell:
nasm -f win64 imgCvtGrayDoubleToInt.asm -o imgCvtGrayDoubleToInt.obj; if ($?) { gcc main.c imgCvtGrayDoubleToInt.obj -o main.exe }; if ($?) { .\main.exe }

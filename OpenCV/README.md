## Download/Install OpenCV
1. Go to the OpenCV website http://opencv.org/
2. At the top of the page, click on the Releases tab.
3. Under the most recent versions (The current versions is 3.2.0) click on the associated OS pack. We all use Windows and the rest of this installation will occur in Windows. Thus click on Win Pack. I honestly do not know if there are differences for Mac or Android versions. 
4. Clicking on Win Pack will automatically direct you to a sourceforge.net download page and the opencv library will automatically begin downloading. Do not be alarmed by the automatic download. This is supposed to happen. The file that is downloaded will be a opencv-3.2.0-vc14.exe file. Do not be alarmed by the fact you just downloaded an .exe file. This is supposed to happen.
5. Next go to your downloads folder and find the downloaded opencv-3.2.0-vc14.exe file. This .exe file runs 7zip to extract the opencv library to your desired location. Select a good location in your computer for the opencv files to remain. We will need to get into these folders later. If you do not have 7zip, you may need to download it. You can download 7zip here http://www.7-zip.org/
6. Ta-Dah you have OpenCV on your computer now. Seems easy right? Too bad, you can't use it yet.
7. The next step is to go into the opencv directory that was created when you unzipped the downloaded file. Click into the directory, then go into the "build" directory. 
8. Next go into the "python" directory, then the "2.7" directory and then the "x64" directory. The x64 file worked on the version of Anaconda I downloaded, the x86 did not.
9. Copy the cv2.pyd file that is in the x64 directory. Keep it safe, it is now your precious.
10. Next find the directory that Anaconda created when you downloaded and installed Anaconda to run your python scripts. Hopefully it is a folder in your C: drive named Anaconda. If it isn't maybe move it to a folder in your C: drive named Anaconda. 
11. Inside the Anaconda folder, go to the directory called "Lib"
12. Inside "Lib" is a plethora of directories and files that you do not want to mess with. That is why the next move is to go into the directory called "site-packages" and we are going to mess with that
13. Once inside the "site-packages" directory you will find once again an excess amount of directories and files. Do not mess with any of these files. Hopefully up until this point you have kept that cv2.pyd file copied. Paste it into the "site-packages" directory.
14. You read that last line right. Once inside "site-packages" just hit Ctrl-v. Ta-dah. 
15. Once you have pasted it in, check the directory to make sure it is in there. The cv2.pyd file that I have is 43,382 KB in size (for reference)
16. You are now ready to do some OpenCV in python using Anaconda & PyCharm. All you will need to do is import numpy and cv2 at the beginning of the script in order to use the functions. 

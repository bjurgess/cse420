all :	mMulti_NoLocks mMulti_CPLocks

mMulti_NoLocks : mMulti_NoLocks.o 
	g++ mMulti_NoLocks.o -o mMulti_NoLocks -L/usr/local/lib -L/user/cse420/Public/lib -lboost_system -lboost_thread

mMulti_NoLocks.o : mMulti_NoLocks.cpp
	g++ -c mMulti_NoLocks.cpp  -I /user/cse420/Public/include

mMulti_CPLocks : mMulti_CPLocks.o
	g++ mMulti_CPLocks.o -o mMulti_CPLocks -L/usr/local/lib -L/user/cse420/Public/lib -lboost_system -lboost_thread

mMulti_CPLocks.o : mMulti_CPLocks.cpp 
	g++ -c mMulti_CPLocks.cpp  -I /user/cse420/Public/include
	
clean :
	rm mMulti_NoLocks mMulti_CPLocks *.o mMulti_NoLocks.exe mMulti_CPLocks.exe

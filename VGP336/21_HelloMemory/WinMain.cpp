#include <NULLEngine/Inc/NULLEngine.h>

using namespace NULLEngine;
using namespace NULLEngine::Core;

class Student
{
public:
    Student() = default;
  
    Student(const std::string& name)
        : mName(name)
    {

    }

    Student(const std::string& name, int classMark)
        : mName(name)
        , mClassMark(classMark)
    {

    }

    void SetName(const std::string& name)
    {
        mName = name;
    }

    void SetGrade(int classMark)
    {
        mClassMark = classMark;
    }

private:
    std::string mName;
    int mClassMark;
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 100);
    Student* a = studentPool.New("Frank");
    Student* b = studentPool.New("Leon", 120);
    Student* c = studentPool.New("Leon", 120);
    Student* d = studentPool.New("Leon", 120);
    // a,b,c,d

    studentPool.Delete(c);
    // a,b, _, d

    Student* e = studentPool.New("Leon", 120);
    // a, b, e, d

    Student* f = studentPool.New("Leon", 120);
    Student* g = studentPool.New("Leon", 120);


    studentPool.Delete(a);
    studentPool.Delete(b);

    return 0;
}
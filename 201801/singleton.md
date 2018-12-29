构造函数声明为private或protect防止被外部函数实例化，内部保存一个private static的类指针保存唯一的实例，实例的动作由一个public的类方法代劳，该方法也返回单例类唯一的实例。
```
class singleton
{
protected:
    singleton(){}
private:
    static singleton* p;
public:
    static singleton* instance();
};

singleton* singleton::p = NULL;
singleton* singleton::instance()
{
    if (p == NULL)
        p = new singleton();
    return p;
}

```
但是很显然，instance函数不是线程安全的.


2. 懒汉与饿汉

单例大约有两种实现方法：懒汉与饿汉。
* 懒汉：故名思义，不到万不得已就不会去实例化类，也就是说在第一次用到类实例的时候才会去实例化，所以上边的经典方法被归为懒汉实现；
* 饿汉：饿了肯定要饥不择食。所以在单例类定义的时候就进行实例化。

下面是on deman的实现方式，关键点：double check
```
class singleton
{
protected:
    singleton()
    {
        pthread_mutex_init(&mutex);
    }
private:
    static singleton* p;
public:
    static pthread_mutex_t mutex;
    static singleton* initance();
};

pthread_mutex_t singleton::mutex;
singleton* singleton::p = NULL;
singleton* singleton::initance()
{
    if (p == NULL)
    {
        pthread_mutex_lock(&mutex);
        if (p == NULL)
            p = new singleton();
        pthread_mutex_unlock(&mutex);
    }
    return p;
}
```
当然，我们也可以采用静态局部变量的方式来实现.
采用静态变量，是为了保证单例；局部变量，是为了实现延迟初始化；

```
class singleton
{
protected:
    singleton()
    {
        pthread_mutex_init(&mutex);
    }
public:
    static pthread_mutex_t mutex;
    static singleton* initance();
    int a;
};

pthread_mutex_t singleton::mutex;
singleton* singleton::initance()
{
    pthread_mutex_lock(&mutex);
    static singleton obj;
    pthread_mutex_unlock(&mutex);
    return &obj;
}
```
看了上述代码，估计你会很奇怪，为什么此处需要加锁？难道静态局部变量的赋值不是线程安全吗？

也就是说，从C++11之后，我们可以直接remove掉lock和unlock的部分。具体原理可以参考这里(https://manishearth.github.io/blog/2015/06/26/adventures-in-systems-programming-c-plus-plus-local-statics/)


饿汉实现

恶汉方式无非是提前实例化，相当简单。

```
class singleton
{
protected:
    singleton()
    {}
private:
    static singleton* p;
public:
    static singleton* initance();
};
singleton* singleton::p = new singleton;
singleton* singleton::initance()
{
    return p;
}
```

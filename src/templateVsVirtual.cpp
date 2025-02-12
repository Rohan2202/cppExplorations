#include <iostream>
#include <chrono>

//  Default Template (No-Op, should be optimized out)
class NoOp
{
public:
    __attribute__((always_inline)) void compute(long int &) {} // Empty function (should be removed)
};

//  Specialized Template (Performs an Operation)
class ComputeWork
{
public:
    __attribute__((always_inline)) void compute(long int &x)
    {
        x = (x + 1012) % 10007;
    }
};

//  Template Class
template <typename T = NoOp>
class ComputeTemplate
{
    T obj;
    long int n;

public:
    ComputeTemplate(long int n) : n(n) {}

    void run(long int &x)
    {
        obj.compute(x);
    }
};

//  Virtual Function Example
class VirtualBase
{
protected:
    long int n;

public:
    VirtualBase(long int n) : n(n) {}
    virtual void run(long int &) {}
};

class VirtualDerivedEmpty : public VirtualBase
{
public:
    VirtualDerivedEmpty(long int n) : VirtualBase(n) {}

    void run(long int &) override {} // Overriding but still empty
};

class VirtualDerived final : public VirtualBase
{
public:
    VirtualDerived(long int n) : VirtualBase(n) {}

    void run(long int &x) override
    {
        x = (x + 1012) % 10007;
    }
};

//  Benchmark Function for Template and Virtual Execution Time
template <typename T>
void benchmark(const std::string &name, T &obj)
{
    long int x = 0;
    long int n = 100000000;
    auto start = std::chrono::high_resolution_clock::now();
    for (long int i = 0; i < n; ++i)
    {
        obj.run(x);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << name << " Time: " << duration.count() << " seconds. Value: " << x << std::endl;
}

//  Main Function
int main()
{
    long int n = 100000000;

    {
        VirtualBase vb(n); // No-op case (no virtual override)
        benchmark("Virtual Function (VirtualBase - No Override)", vb);

        VirtualDerivedEmpty vde(n); // No-op case (no virtual override)
        benchmark("Virtual Function (VirtualDerived No-Op in override)", vb);

        //  Benchmark Virtual Function Performance
        VirtualDerived vd(n);
        benchmark("Virtual Function (VirtualDerived)", vd);
    }

    {                                     //  Benchmark Template Performance
        ComputeTemplate<> noOpCompute(n); //  Default template (should be optimized out)
        benchmark("Template (NoOp)", noOpCompute);

        ComputeTemplate<ComputeWork> workCompute(n); //  Template with computation
        benchmark("Template (ComputeWork)", workCompute);
    }

    return 0;
}

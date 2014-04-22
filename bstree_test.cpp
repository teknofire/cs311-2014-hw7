// bstree_test.cpp
// Glenn G. Chappell
// 20 Nov 2009
//
// For CS 311 Fall 2009
// Test program for class template BSTree
// Used in Assignment 7, Exercise A

// Includes for code to be tested
#include "bstree.h"     // For class template BSTree
#include "bstree.h"     // Double inclusion test

// Includes for testing package & code common to all test programs
#include <iostream>     // for std::cout, std::endl, std::cin
#include <string>       // for std::string
#include <stdexcept>    // for std::runtime_error

// Additional includes for this test program
#include <cstdlib>      // for std::size_t
#include <vector>       // for std::vector


// ************************************************************************
// Testing Package:
//     Class Tester - For Tracking Tests
// ************************************************************************


// class Tester
// For extremely simple unit testing.
// Keeps track of number of tests and number of passes.
// Use test (with success/failure parameter) to do a test.
// Get results with numTests, numPassed, numFailed, allPassed.
// Restart testing with reset.
// Invariants:
//     countTests_ == number of tests (calls to test) since last reset.
//     countPasses_ == number of times function test called with true param
//      since last reset.
//     0 <= countPasses_ <= countTests_.
//     tolerance_ >= 0.
class Tester {

// ***** Tester: ctors, dctor, op= *****
public:

    // Default ctor
    // Sets countTests_, countPasses_ to zero, tolerance_ to given value
    // Pre: None.
    // Post:
    //     numTests == 0, countPasses == 0, tolerance_ == abs(theTolerance)
    // Does not throw (No-Throw Guarantee)
    Tester(double theTolerance = 0.0000001)
        :countTests_(0),
         countPasses_(0),
         tolerance_(theTolerance >= 0 ? theTolerance : -theTolerance)
    {}

    // Compiler-generated copy ctor, copy op=, dctor are used

// ***** Tester: general public functions *****
public:

    // test
    // Handles single test, param indicates pass/fail
    // Pre: None.
    // Post:
    //     countTests_ incremented
    //     countPasses_ incremented if (success)
    //     Message indicating test name (if given)
    //      and pass/fail printed to cout
    // Does not throw (No-Throw Guarantee)
    //  - Assuming exceptions have not been turned on for cout.
    void test(bool success,
              const std::string & testName = "")
    {
        ++countTests_;
        if (success) ++countPasses_;

        std::cout << "    ";
        if (testName != "")
        {
            std::cout << "Test: "
                      << testName
                      << " - ";
        }
        std::cout << (success ? "passed" : "********** FAILED **********")
                  << std::endl;
    }

    // ftest
    // Does single floating-point test.
    // Tests passes iff difference of first two values is <= tolerance.
    // Pre: None.
    // Post:
    //     countTests_ incremented
    //     countPasses_ incremented if (abs(val1-val2) <= tolerance_)
    //     Message indicating test name (if given)
    //      and pass/fail printed to cout
    // Does not throw (No-Throw Guarantee)
    void ftest(double val1,
               double val2,
               const std::string & testName = "")
    { test(val1-val2 <= tolerance_ && val2-val1 <= tolerance_, testName); }

    // reset
    // Resets *this to default constructed state
    // Pre: None.
    // Post:
    //     countTests_ == 0, countPasses_ == 0
    // Does not throw (No-Throw Guarantee)
    void reset()
    {
        countTests_ = 0;
        countPasses_ = 0;
    }

    // numTests
    // Returns the number of tests that have been done since last reset 
    // Pre: None.
    // Post:
    //     return == countTests_
    // Does not throw (No-Throw Guarantee)
    int numTests() const
    { return countTests_; }

    // numPassed
    // Returns the number of tests that have passed since last reset
    // Pre: None.
    // Post:
    //     return == countPasses_
    // Does not throw (No-Throw Guarantee)
    int numPassed() const
    { return countPasses_; }

    // numFailed
    // Returns the number of tests that have not passed since last reset
    // Pre: None.
    // Post:
    //     return + countPasses_ == numTests_
    // Does not throw (No-Throw Guarantee)
    int numFailed() const
    { return countTests_ - countPasses_; }

    // allPassed
    // Returns true if all tests since last reset have passed
    // Pre: None.
    // Post:
    //     return == (countPasses_ == countTests_)
    // Does not throw (No-Throw Guarantee)
    bool allPassed() const
    { return countPasses_ == countTests_; }

    // setTolerance
    // Sets tolerance_ to given value
    // Pre: None.
    // Post:
    //     tolerance_ = abs(theTolerance)
    // Does not throw (No-Throw Guarantee)
    void setTolerance(double theTolerance)
    { tolerance_ = (theTolerance >= 0 ? theTolerance : -theTolerance); }

// ***** Tester: data members *****
private:

    int countTests_;    // Number of tests done since last reset
    int countPasses_;   // Number of tests passed since last reset
    double tolerance_;  // Tolerance for floating-point near-equality tests

};  // end class Tester


// ************************************************************************
// Testing Package:
//     Class TypeCheck - Helper Class for Type Checking
// ************************************************************************


// class TypeCheck
// This class exists in order to have static member function check, which
// takes a parameter of a given type, by reference. Objects of type
// TypeCheck<T> cannot be created.
// Usage:
//     TypeCheck<MyType>::check(x)
//     returns true if the type of x is (MyType) or (const MyType),
//     otherwise false.
// Invariants: None.
// Requirements on Types: None.
template<typename T>
class TypeCheck {

private:

    // Uncopyable class. Do not define copy ctor, copy assn.
    TypeCheck(const TypeCheck &);
    TypeCheck<T> & operator=(const TypeCheck &);

    // Compiler-generated dctor is used (but irrelevant).

public:

    // check
    // The function and function template below simulate a single function
    // that takes a single parameter, and returns true iff the parameter has
    // type T or (const T).

    // check (reference-to-const T)
    // Pre: None.
    // Post:
    //     Return is true.
    // Does not throw (No-Throw Guarantee)
    static bool check(const T & param)
    { return true; }

    // check (reference-to-const non-T)
    // Pre: None.
    // Post:
    //     Return is false.
    // Requirements on types: None.
    // Does not throw (No-Throw Guarantee)
    template <typename OtherType>
    static bool check(const OtherType & param)
    { return false; }

};  // End class TypeCheck


// ************************************************************************
// Testing Package:
//     Class Counter - Helper Class for Counting Calls & Objects, Throwing
// ************************************************************************


// class Counter
// Item type for counting ctor, dctor, op= calls, counting existing
//  objects, and possibly throwing on copy. Has operator< (which always
//  returns false), allowing it to be the value type of a sorted container.
// If static member copyThrow_ is set, then copy ctor and copy assn throw
//  std::runtime_error. Exception object's "what" member is set to "C" by
//  the copy ctor and "A" by copy assn.
// Increments static data member ctorCount_ on default construction and
//  successful copy construction. Increments static data member assnCount_
//  on successful copy assignment. Increments static data member
//  dctorCount_ on destruction.
// Increments static data member existing_ on construction, and decrements
//  it on destruction.
// Static data member maxExisting_ is highest value of existing_ since last
//  reset, or start of program if reset has never been called.
// Invariants:
//     Counter::existing_ is number of existing objects of this class.
//     Counter::ctorCount_ is number of successful ctor calls since
//      most recent call to reset, or start of program if reset has never
//      been called.
//     Counter::dctorCount_ is (similarly) number of dctor calls.
//     Counter::assnCount_ is (similarly) number of copy assn calls.
//     Counter::maxExisting_ is (similarly) highest value existing_ has
//      assumed.
class Counter {

// ***** Counter: Ctors, dctor, op= *****
public:

    // Default ctor
    // Pre: None.
    // Post:
    //     (ctorCount_ has been incremented.)
    //     (existing_ has been incremented.)
    // Does not throw (No-Throw Guarantee)
    Counter()
    {
        ++existing_;
        if (existing_ > maxExisting_)
            maxExisting_ = existing_;
        ++ctorCount_;
    }

    // Copy ctor
    // Throws std::runtime_error if copyThrow_.
    // Pre: None.
    // Post:
    //     (ctorCount_ has been incremented.)
    //     (existing_ has been incremented.)
    // May throw std::runtime_error
    // Strong Guarantee
    Counter(const Counter & other)
    {
        if (copyThrow_)
            throw std::runtime_error("C");
        ++existing_;
        if (existing_ > maxExisting_)
            maxExisting_ = existing_;
        ++ctorCount_;
    }

    // Copy assignment
    // Throws std::runtime_error if copyThrow_.
    // Pre: None.
    // Post:
    //     Return value is *this.
    //     (assnCount_ has been incremented.)
    // May throw std::runtime_error
    // Strong Guarantee
    Counter & operator=(const Counter & rhs)
    {
        if (copyThrow_)
            throw std::runtime_error("A");
        ++assnCount_;
        return *this;
    }

    // Dctor
    // Pre: None.
    // Post:
    //     (dctorCount_ has been incremented.)
    //     (existing_ has been decremented.)
    // Does not throw (No-Throw Guarantee)
    ~Counter()
    {
        --existing_;
        ++dctorCount_;
    }

// ***** Counter: Functions dealing with count *****
public:

    // reset
    // Pre: None.
    // Post:
    //     maxExisting_ == existing_.
    //     ctorCount_ == 0.
    //     dctorCount_ == 0.
    //     assnCount_ == 0.
    //     copyThrow_ == shouldThrow.
    // Does not throw (No-Throw Guarantee)
    static void reset(bool shouldThrow = false)
    {
        maxExisting_ = existing_;
        ctorCount_ = 0;
        dctorCount_ = 0;
        assnCount_ = 0;
        copyThrow_ = shouldThrow;
    }

    // getExisting
    // Pre: None.
    // Post:
    //     return == existing_.
    // Does not throw (No-Throw Guarantee)
    static int getExisting()
    { return existing_; }

    // getMaxExisting
    // Pre: None.
    // Post:
    //     return == maxExisting_.
    // Does not throw (No-Throw Guarantee)
    static int getMaxExisting()
    { return maxExisting_; }

    // getCtorCount
    // Pre: None.
    // Post:
    //     return == ctorCount_.
    // Does not throw (No-Throw Guarantee)
    static int getCtorCount()
    { return ctorCount_; }

    // getDctorCount
    // Pre: None.
    // Post:
    //     return == dctorCount_.
    // Does not throw (No-Throw Guarantee)
    static int getDctorCount()
    { return dctorCount_; }

    // getAssnCount
    // Pre: None.
    // Post:
    //     return == assnCount_.
    // Does not throw (No-Throw Guarantee)
    static int getAssnCount()
    { return assnCount_; }

    // setCopyThrow
    // Pre: None.
    // Post:
    //     copyThrow_ == shouldThrow
    // Does not throw (No-Throw Guarantee)
    static void setCopyThrow(bool shouldThrow)
    { copyThrow_ = shouldThrow; }

// ***** Counter: Data Members *****
private:

    static int existing_;     // # of existing objects
    static int maxExisting_;  // Max # of existing objects
    static int ctorCount_;    // # of successful (non-throwing) ctor calls
    static int dctorCount_;   // # of dctor calls
    static int assnCount_;    // # of successful (non-throwing) copy = calls
    static bool copyThrow_;   // true if copy operations (ctor, =) throw

};  // End class Counter

// Definition of static data member of class Counter
int Counter::existing_ = 0;
int Counter::maxExisting_ = 0;
int Counter::ctorCount_ = 0;
int Counter::dctorCount_ = 0;
int Counter::assnCount_ = 0;
bool Counter::copyThrow_ = false;


// operator< (Counter)
// Dummy-ish operator<, forming a strict weak order for Counter class
// Returns false (which is legal for a strict weak order; all objects of
//  type Counter are equivalent).
// Pre: None.
// Post:
//     Return value == false.
// Does not throw (No-Throw Guarantee)
bool operator<(const Counter & a,
               const Counter & b)
{ return false; }


// ************************************************************************
// Test Suite Functions
// ************************************************************************


// test_class_BSTree_types
// Test suite for class BSTree, types
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_BSTree_types(Tester & t)
{
    std::cout << "Test Suite: class BSTree, types" << std::endl;

    bool correctType;  // result of type checking

    const BSTree<int> cbst1;
    BSTree<int> bst2;

    // MEMBER FUNCTION copy assignment op
    // Check return type
    correctType = TypeCheck<BSTree<int> >::check(bst2 = cbst1);
    t.test(correctType, "Copy assignment operator (const), return type");
    correctType = TypeCheck<BSTree<int> >::check(bst2 = bst2);
    t.test(correctType, "Copy assignment operator (non-const), return type");
    (bst2 = cbst1) = cbst1;  // Return type modifiable? (only needs to compile)
    t.test(true, "Copy assignment operator, return value modifiable");

    // MEMBER FUNCTION size
    // Check return type
    correctType = TypeCheck<std::size_t>::check(cbst1.size());
    t.test(correctType, "size, return type");

    // MEMBER FUNCTION empty
    // Check return type
    correctType = TypeCheck<bool>::check(cbst1.empty());
    t.test(correctType, "empty, return type");

    // MEMBER FUNCTION retrieve
    // Check return type
    correctType = TypeCheck<bool>::check(cbst1.retrieve(1));
    t.test(correctType, "retrieve, return type");

    // MEMBER FUNCTION insert
    // Check return type
    correctType = TypeCheck<bool>::check(bst2.insert(1));
    t.test(correctType, "insert, return type");
}


// test_class_BSTree_very_small
// Test suite for class BSTree, very small trees
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_BSTree_very_small(Tester & t)
{
    std::cout << "Test Suite: class BSTree, very small trees" << std::endl;

    BSTree<int> bst1;
    const BSTree<int> & cbstr = bst1;
    std::vector<int> v;  // Holds values from traversals

    // Empty tree
    t.test(cbstr.size() == 0, "Default constructed, size");
    t.test(cbstr.empty(), "Default constructed, empty");
    t.test(!cbstr.retrieve(1), "Default constructed, retrieve #1");
    t.test(!cbstr.retrieve(2), "Default constructed, retrieve #2");
    t.test(!cbstr.retrieve(3), "Default constructed, retrieve #3");
    t.test(!cbstr.retrieve(1), "Default constructed, retrieve #1 (repeat)");
    v.assign(3,5);
    cbstr.preorderTraverse(v.begin());
    t.test(v[0] == 5 && v[1] == 5 && v[2] == 5, "Default constructed, preorder");
    v.assign(3,5);
    cbstr.inorderTraverse(v.begin());
    t.test(v[0] == 5 && v[1] == 5 && v[2] == 5, "Default constructed, inorder");
    v.assign(3,5);
    cbstr.postorderTraverse(v.begin());
    t.test(v[0] == 5 && v[1] == 5 && v[2] == 5, "Default constructed, postorder");
    t.test(bst1.insert(1), "Default constructed, insert return value");

    // Singleton tree
    t.test(cbstr.size() == 1, "Singleton tree, size");
    t.test(!cbstr.empty(), "Singleton tree, empty");
    t.test(cbstr.retrieve(1), "Singleton tree, retrieve #1");
    t.test(!cbstr.retrieve(2), "Singleton tree, retrieve #2");
    t.test(!cbstr.retrieve(3), "Singleton tree, retrieve #3");
    v.assign(3,5);
    cbstr.preorderTraverse(v.begin());
    t.test(v[0] == 1 && v[1] == 5 && v[2] == 5, "Singleton tree, preorder");
    v.assign(3,5);
    cbstr.inorderTraverse(v.begin());
    t.test(v[0] == 1 && v[1] == 5 && v[2] == 5, "Singleton tree, inorder");
    v.assign(3,5);
    cbstr.postorderTraverse(v.begin());
    t.test(v[0] == 1 && v[1] == 5 && v[2] == 5, "Singleton tree, postorder");
    bst1.clear();

    // Empty tree again
    t.test(cbstr.size() == 0, "Empty tree again, size");
    t.test(cbstr.empty(), "Empty tree again, empty");
    t.test(!cbstr.retrieve(1), "Empty tree again, retrieve #1");
    t.test(!cbstr.retrieve(2), "Empty tree again, retrieve #2");
    t.test(!cbstr.retrieve(3), "Empty tree again, retrieve #3");
    v.assign(3,5);
    cbstr.preorderTraverse(v.begin());
    t.test(v[0] == 5 && v[1] == 5 && v[2] == 5, "Empty tree again, preorder");
    v.assign(3,5);
    cbstr.inorderTraverse(v.begin());
    t.test(v[0] == 5 && v[1] == 5 && v[2] == 5, "Empty tree again, inorder");
    v.assign(3,5);
    cbstr.postorderTraverse(v.begin());
    t.test(v[0] == 5 && v[1] == 5 && v[2] == 5, "Empty tree again, postorder");
    t.test(bst1.insert(2), "Empty tree again, insert return value");

    // Singleton tree again
    t.test(cbstr.size() == 1, "Singleton tree again, size");
    t.test(!cbstr.empty(), "Singleton tree again, empty");
    t.test(!cbstr.retrieve(1), "Singleton tree again, retrieve #1");
    t.test(cbstr.retrieve(2), "Singleton tree again, retrieve #2");
    t.test(!cbstr.retrieve(3), "Singleton tree again, retrieve #3");
    t.test(!bst1.insert(2), "Singleton tree again, re-insert same key");
    t.test(cbstr.size() == 1, "Singleton tree again, size");
    t.test(!cbstr.empty(), "Singleton tree again, empty");
    t.test(!cbstr.retrieve(1), "Singleton tree again, retrieve #1");
    t.test(cbstr.retrieve(2), "Singleton tree again, retrieve #2");
    t.test(!cbstr.retrieve(3), "Singleton tree again, retrieve #3");
    v.assign(3,5);
    cbstr.preorderTraverse(v.begin());
    t.test(v[0] == 2 && v[1] == 5 && v[2] == 5, "Singleton tree again, preorder");
    v.assign(3,5);
    cbstr.inorderTraverse(v.begin());
    t.test(v[0] == 2 && v[1] == 5 && v[2] == 5, "Singleton tree again, inorder");
    v.assign(3,5);
    cbstr.postorderTraverse(v.begin());
    t.test(v[0] == 2 && v[1] == 5 && v[2] == 5, "Singleton tree again, postorder");
    t.test(bst1.insert(1), "Singleton tree gain, insert new value");

    // 2-item tree
    t.test(cbstr.size() == 2, "2-item tree, size");
    t.test(!cbstr.empty(), "2-item tree, empty");
    t.test(cbstr.retrieve(1), "2-item tree, retrieve #1");
    t.test(cbstr.retrieve(2), "2-item tree, retrieve #2");
    t.test(!cbstr.retrieve(3), "2-item tree, retrieve #3");
    t.test(!bst1.insert(2), "2-item tree, re-insert same key");
    t.test(cbstr.size() == 2, "2-item tree, size");
    t.test(!cbstr.empty(), "2-item tree, empty");
    t.test(cbstr.retrieve(1), "2-item tree, retrieve #1");
    t.test(cbstr.retrieve(2), "2-item tree, retrieve #2");
    t.test(!cbstr.retrieve(3), "2-item tree, retrieve #3");
    v.assign(3,5);
    cbstr.preorderTraverse(v.begin());
    t.test(v[0] == 2 && v[1] == 1 && v[2] == 5, "2-item tree, preorder");
    v.assign(3,5);
    cbstr.inorderTraverse(v.begin());
    t.test(v[0] == 1 && v[1] == 2 && v[2] == 5, "2-item tree, inorder");
    v.assign(3,5);
    cbstr.postorderTraverse(v.begin());
    t.test(v[0] == 1 && v[1] == 2 && v[2] == 5, "2-item tree, postorder");
    bst1.clear();

    // Empty tree yet again
    t.test(cbstr.size() == 0, "Empty tree yet again, size");
    t.test(cbstr.empty(), "Empty tree yet again, empty");
    t.test(!cbstr.retrieve(1), "Empty tree yet again, retrieve #1");
    t.test(!cbstr.retrieve(2), "Empty tree yet again, retrieve #2");
    t.test(!cbstr.retrieve(3), "Empty tree yet again, retrieve #3");
    v.assign(3,5);
    cbstr.preorderTraverse(v.begin());
    t.test(v[0] == 5 && v[1] == 5 && v[2] == 5, "Empty tree yet again, preorder");
    v.assign(3,5);
    cbstr.inorderTraverse(v.begin());
    t.test(v[0] == 5 && v[1] == 5 && v[2] == 5, "Empty tree yet again, inorder");
    v.assign(3,5);
    cbstr.postorderTraverse(v.begin());
    t.test(v[0] == 5 && v[1] == 5 && v[2] == 5, "Empty tree yet again, postorder");
}


// test_class_BSTree_larger
// Test suite for class BSTree, larger trees
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_BSTree_larger(Tester & t)
{
    std::cout << "Test Suite: class BSTree, larger trees" << std::endl;

    BSTree<int> bst1;
    std::vector<int> v;  // Holds values from traversals

    bst1.insert(50);
    bst1.insert(20);
    bst1.insert(70);
    bst1.insert(60);
    bst1.insert(10);
    bst1.insert(30);
    bst1.insert(80);
    bst1.insert(65);

    t.test(bst1.size() == 8, "Larger tree, size");
    t.test(!bst1.empty(), "Larger tree, empty");
    t.test(bst1.retrieve(65), "Larger tree, retrieve #1");
    t.test(!bst1.retrieve(25), "Larger tree, retrieve #2");
    t.test(!bst1.retrieve(55), "Larger tree, retrieve #3");
    v.assign(9, 6);
    bst1.preorderTraverse(v.begin());
    t.test(v[0] == 50 && v[1] == 20 && v[2] == 10
        && v[3] == 30 && v[4] == 70 && v[5] == 60
        && v[6] == 65 && v[7] == 80 && v[8] == 6, "Larger tree, preorder");
    v.assign(9, 6);
    bst1.inorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 20 && v[2] == 30
        && v[3] == 50 && v[4] == 60 && v[5] == 65
        && v[6] == 70 && v[7] == 80 && v[8] == 6, "Larger tree, inorder");
    v.assign(9, 6);
    bst1.postorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 30 && v[2] == 20
        && v[3] == 65 && v[4] == 60 && v[5] == 80
        && v[6] == 70 && v[7] == 50 && v[8] == 6, "Larger tree, postorder");

    t.test(!bst1.insert(65), "Larger tree, insert existing item");

    t.test(bst1.size() == 8, "Same larger tree, size");
    t.test(!bst1.empty(), "Same larger tree, empty");
    t.test(bst1.retrieve(65), "Same larger tree, retrieve #1");
    t.test(!bst1.retrieve(25), "Same larger tree, retrieve #2");
    t.test(!bst1.retrieve(55), "Same larger tree, retrieve #3");
    v.assign(9, 6);
    bst1.preorderTraverse(v.begin());
    t.test(v[0] == 50 && v[1] == 20 && v[2] == 10
        && v[3] == 30 && v[4] == 70 && v[5] == 60
        && v[6] == 65 && v[7] == 80 && v[8] == 6, "Same larger tree, preorder");
    v.assign(9, 6);
    bst1.inorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 20 && v[2] == 30
        && v[3] == 50 && v[4] == 60 && v[5] == 65
        && v[6] == 70 && v[7] == 80 && v[8] == 6, "Same larger tree, inorder");
    v.assign(9, 6);
    bst1.postorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 30 && v[2] == 20
        && v[3] == 65 && v[4] == 60 && v[5] == 80
        && v[6] == 70 && v[7] == 50 && v[8] == 6, "Same larger tree, postorder");

    t.test(bst1.insert(25), "Same larger tree, insert new item");

    t.test(bst1.size() == 9, "Yet larger tree, size");
    t.test(!bst1.empty(), "Yet larger tree, empty");
    t.test(bst1.retrieve(65), "Yet larger tree, retrieve #1");
    t.test(bst1.retrieve(25), "Yet larger tree, retrieve #2");
    t.test(!bst1.retrieve(55), "Yet larger tree, retrieve #3");
    v.assign(10, 7);
    bst1.preorderTraverse(v.begin());
    t.test(v[0] == 50 && v[1] == 20 && v[2] == 10
        && v[3] == 30 && v[4] == 25 && v[5] == 70
        && v[6] == 60 && v[7] == 65 && v[8] == 80
        && v[9] == 7, "Yet larger tree, preorder");
    v.assign(10, 7);
    bst1.inorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 20 && v[2] == 25
        && v[3] == 30 && v[4] == 50 && v[5] == 60
        && v[6] == 65 && v[7] == 70 && v[8] == 80
        && v[9] == 7, "Yet larger tree, inorder");
    v.assign(10, 7);
    bst1.postorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 25 && v[2] == 30
        && v[3] == 20 && v[4] == 65 && v[5] == 60
        && v[6] == 80 && v[7] == 70 && v[8] == 50
        && v[9] == 7, "Yet larger tree, postorder");
}


// test_class_BSTree_copy_ctor
// Test suite for class BSTree, copy ctor
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_BSTree_copy_ctor(Tester & t)
{
    std::cout << "Test Suite: class BSTree, copy constructor" << std::endl;

    BSTree<int> bst1;
    std::vector<int> v;  // Holds values from traversals

    bst1.insert(50);
    bst1.insert(20);
    bst1.insert(70);
    bst1.insert(60);
    bst1.insert(10);
    bst1.insert(30);
    bst1.insert(80);
    bst1.insert(65);
    const BSTree<int> & cbstr = bst1;

    BSTree<int> bst2 = cbstr;

    t.test(bst2.size() == 8, "Copy constructed, size");
    t.test(!bst2.empty(), "Copy constructed, empty");
    t.test(bst2.retrieve(65), "Copy constructed, retrieve #1");
    t.test(!bst2.retrieve(25), "Copy constructed, retrieve #2");
    t.test(!bst2.retrieve(55), "Copy constructed, retrieve #3");
    v.assign(9, 6);
    bst2.preorderTraverse(v.begin());
    t.test(v[0] == 50 && v[1] == 20 && v[2] == 10
        && v[3] == 30 && v[4] == 70 && v[5] == 60
        && v[6] == 65 && v[7] == 80 && v[8] == 6, "Copy constructed, preorder");
    v.assign(9, 6);
    bst2.inorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 20 && v[2] == 30
        && v[3] == 50 && v[4] == 60 && v[5] == 65
        && v[6] == 70 && v[7] == 80 && v[8] == 6, "Copy constructed, inorder");
    v.assign(9, 6);
    bst2.postorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 30 && v[2] == 20
        && v[3] == 65 && v[4] == 60 && v[5] == 80
        && v[6] == 70 && v[7] == 50 && v[8] == 6, "Copy constructed, postorder");

    t.test(bst1.insert(25), "Original, insert new item");

    t.test(bst1.size() == 9, "Modified original, size");
    t.test(!bst1.empty(), "Modified original, empty");
    t.test(bst1.retrieve(65), "Modified original, retrieve #1");
    t.test(bst1.retrieve(25), "Modified original, retrieve #2");
    t.test(!bst1.retrieve(55), "Modified original, retrieve #3");
    v.assign(10, 7);
    bst1.preorderTraverse(v.begin());
    t.test(v[0] == 50 && v[1] == 20 && v[2] == 10
        && v[3] == 30 && v[4] == 25 && v[5] == 70
        && v[6] == 60 && v[7] == 65 && v[8] == 80
        && v[9] == 7, "Modified original, preorder");
    v.assign(10, 7);
    bst1.inorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 20 && v[2] == 25
        && v[3] == 30 && v[4] == 50 && v[5] == 60
        && v[6] == 65 && v[7] == 70 && v[8] == 80
        && v[9] == 7, "Modified original, inorder");
    v.assign(10, 7);
    bst1.postorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 25 && v[2] == 30
        && v[3] == 20 && v[4] == 65 && v[5] == 60
        && v[6] == 80 && v[7] == 70 && v[8] == 50
        && v[9] == 7, "Modified original, postorder");

    t.test(bst2.size() == 8, "Copy retest, size");
    t.test(!bst2.empty(), "Copy retest, empty");
    t.test(bst2.retrieve(65), "Copy retest, retrieve #1");
    t.test(!bst2.retrieve(25), "Copy retest, retrieve #2");
    t.test(!bst2.retrieve(55), "Copy retest, retrieve #3");
    v.assign(9, 6);
    bst2.preorderTraverse(v.begin());
    t.test(v[0] == 50 && v[1] == 20 && v[2] == 10
        && v[3] == 30 && v[4] == 70 && v[5] == 60
        && v[6] == 65 && v[7] == 80 && v[8] == 6, "Copy retest, preorder");
    v.assign(9, 6);
    bst2.inorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 20 && v[2] == 30
        && v[3] == 50 && v[4] == 60 && v[5] == 65
        && v[6] == 70 && v[7] == 80 && v[8] == 6, "Copy retest, inorder");
    v.assign(9, 6);
    bst2.postorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 30 && v[2] == 20
        && v[3] == 65 && v[4] == 60 && v[5] == 80
        && v[6] == 70 && v[7] == 50 && v[8] == 6, "Copy retest, postorder");

    bst2.clear();

    t.test(bst2.size() == 0, "Copy cleared, size");
    t.test(bst2.empty(), "Copy cleared, empty");
    t.test(!bst2.retrieve(65), "Copy cleared, retrieve #1");
    t.test(!bst2.retrieve(25), "Copy cleared, retrieve #2");
    t.test(!bst2.retrieve(55), "Copy cleared, retrieve #3");
    v.assign(2, 9);
    bst2.preorderTraverse(v.begin());
    t.test(v[0] == 9 && v[1] == 9, "Copy cleared, preorder");
    v.assign(2, 9);
    bst2.inorderTraverse(v.begin());
    t.test(v[0] == 9 && v[1] == 9, "Copy cleared, inorder");
    v.assign(2, 9);
    bst2.postorderTraverse(v.begin());
    t.test(v[0] == 9 && v[1] == 9, "Copy cleared, postorder");

    t.test(bst1.size() == 9, "Original retest, size");
    t.test(!bst1.empty(), "Original retest, empty");
    t.test(bst1.retrieve(65), "Original retest, retrieve #1");
    t.test(bst1.retrieve(25), "Original retest, retrieve #2");
    t.test(!bst1.retrieve(55), "Original retest, retrieve #3");
    v.assign(10, 7);
    bst1.preorderTraverse(v.begin());
    t.test(v[0] == 50 && v[1] == 20 && v[2] == 10
        && v[3] == 30 && v[4] == 25 && v[5] == 70
        && v[6] == 60 && v[7] == 65 && v[8] == 80
        && v[9] == 7, "Original retest, preorder");
    v.assign(10, 7);
    bst1.inorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 20 && v[2] == 25
        && v[3] == 30 && v[4] == 50 && v[5] == 60
        && v[6] == 65 && v[7] == 70 && v[8] == 80
        && v[9] == 7, "Original retest, inorder");
    v.assign(10, 7);
    bst1.postorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 25 && v[2] == 30
        && v[3] == 20 && v[4] == 65 && v[5] == 60
        && v[6] == 80 && v[7] == 70 && v[8] == 50
        && v[9] == 7, "Original retest, postorder");
}


// test_class_BSTree_copy_assn
// Test suite for class BSTree, copy assn
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_BSTree_copy_assn(Tester & t)
{
    std::cout << "Test Suite: class BSTree, copy assignment" << std::endl;

    BSTree<int> bst1;
    std::vector<int> v;  // Holds values from traversals

    bst1.insert(50);
    bst1.insert(20);
    bst1.insert(70);
    bst1.insert(60);
    bst1.insert(10);
    bst1.insert(30);
    bst1.insert(80);
    bst1.insert(65);
    const BSTree<int> & cbstr = bst1;

    BSTree<int> bst2;
    bst2 = cbstr;

    t.test(bst2.size() == 8, "Copy assigned, size");
    t.test(!bst2.empty(), "Copy assigned, empty");
    t.test(bst2.retrieve(65), "Copy assigned, retrieve #1");
    t.test(!bst2.retrieve(25), "Copy assigned, retrieve #2");
    t.test(!bst2.retrieve(55), "Copy assigned, retrieve #3");
    v.assign(9, 6);
    bst2.preorderTraverse(v.begin());
    t.test(v[0] == 50 && v[1] == 20 && v[2] == 10
        && v[3] == 30 && v[4] == 70 && v[5] == 60
        && v[6] == 65 && v[7] == 80 && v[8] == 6, "Copy assigned, preorder");
    v.assign(9, 6);
    bst2.inorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 20 && v[2] == 30
        && v[3] == 50 && v[4] == 60 && v[5] == 65
        && v[6] == 70 && v[7] == 80 && v[8] == 6, "Copy assigned, inorder");
    v.assign(9, 6);
    bst2.postorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 30 && v[2] == 20
        && v[3] == 65 && v[4] == 60 && v[5] == 80
        && v[6] == 70 && v[7] == 50 && v[8] == 6, "Copy assigned, postorder");

    t.test(bst1.insert(25), "Original, insert new item");

    t.test(bst1.size() == 9, "Modified original, size");
    t.test(!bst1.empty(), "Modified original, empty");
    t.test(bst1.retrieve(65), "Modified original, retrieve #1");
    t.test(bst1.retrieve(25), "Modified original, retrieve #2");
    t.test(!bst1.retrieve(55), "Modified original, retrieve #3");
    v.assign(10, 7);
    bst1.preorderTraverse(v.begin());
    t.test(v[0] == 50 && v[1] == 20 && v[2] == 10
        && v[3] == 30 && v[4] == 25 && v[5] == 70
        && v[6] == 60 && v[7] == 65 && v[8] == 80
        && v[9] == 7, "Modified original, preorder");
    v.assign(10, 7);
    bst1.inorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 20 && v[2] == 25
        && v[3] == 30 && v[4] == 50 && v[5] == 60
        && v[6] == 65 && v[7] == 70 && v[8] == 80
        && v[9] == 7, "Modified original, inorder");
    v.assign(10, 7);
    bst1.postorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 25 && v[2] == 30
        && v[3] == 20 && v[4] == 65 && v[5] == 60
        && v[6] == 80 && v[7] == 70 && v[8] == 50
        && v[9] == 7, "Modified original, postorder");

    t.test(bst2.size() == 8, "Copy retest, size");
    t.test(!bst2.empty(), "Copy retest, empty");
    t.test(bst2.retrieve(65), "Copy retest, retrieve #1");
    t.test(!bst2.retrieve(25), "Copy retest, retrieve #2");
    t.test(!bst2.retrieve(55), "Copy retest, retrieve #3");
    v.assign(9, 6);
    bst2.preorderTraverse(v.begin());
    t.test(v[0] == 50 && v[1] == 20 && v[2] == 10
        && v[3] == 30 && v[4] == 70 && v[5] == 60
        && v[6] == 65 && v[7] == 80 && v[8] == 6, "Copy retest, preorder");
    v.assign(9, 6);
    bst2.inorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 20 && v[2] == 30
        && v[3] == 50 && v[4] == 60 && v[5] == 65
        && v[6] == 70 && v[7] == 80 && v[8] == 6, "Copy retest, inorder");
    v.assign(9, 6);
    bst2.postorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 30 && v[2] == 20
        && v[3] == 65 && v[4] == 60 && v[5] == 80
        && v[6] == 70 && v[7] == 50 && v[8] == 6, "Copy retest, postorder");

    bst2.clear();

    t.test(bst2.size() == 0, "Copy cleared, size");
    t.test(bst2.empty(), "Copy cleared, empty");
    t.test(!bst2.retrieve(65), "Copy cleared, retrieve #1");
    t.test(!bst2.retrieve(25), "Copy cleared, retrieve #2");
    t.test(!bst2.retrieve(55), "Copy cleared, retrieve #3");
    v.assign(2, 9);
    bst2.preorderTraverse(v.begin());
    t.test(v[0] == 9 && v[1] == 9, "Copy cleared, preorder");
    v.assign(2, 9);
    bst2.inorderTraverse(v.begin());
    t.test(v[0] == 9 && v[1] == 9, "Copy cleared, inorder");
    v.assign(2, 9);
    bst2.postorderTraverse(v.begin());
    t.test(v[0] == 9 && v[1] == 9, "Copy cleared, postorder");

    t.test(bst1.size() == 9, "Original retest, size");
    t.test(!bst1.empty(), "Original retest, empty");
    t.test(bst1.retrieve(65), "Original retest, retrieve #1");
    t.test(bst1.retrieve(25), "Original retest, retrieve #2");
    t.test(!bst1.retrieve(55), "Original retest, retrieve #3");
    v.assign(10, 7);
    bst1.preorderTraverse(v.begin());
    t.test(v[0] == 50 && v[1] == 20 && v[2] == 10
        && v[3] == 30 && v[4] == 25 && v[5] == 70
        && v[6] == 60 && v[7] == 65 && v[8] == 80
        && v[9] == 7, "Original retest, preorder");
    v.assign(10, 7);
    bst1.inorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 20 && v[2] == 25
        && v[3] == 30 && v[4] == 50 && v[5] == 60
        && v[6] == 65 && v[7] == 70 && v[8] == 80
        && v[9] == 7, "Original retest, inorder");
    v.assign(10, 7);
    bst1.postorderTraverse(v.begin());
    t.test(v[0] == 10 && v[1] == 25 && v[2] == 30
        && v[3] == 20 && v[4] == 65 && v[5] == 60
        && v[6] == 80 && v[7] == 70 && v[8] == 50
        && v[9] == 7, "Original retest, postorder");
}


// test_class_BSTree
// Test suite for class BSTree
// Uses other test-suite functions
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_BSTree(Tester & t)
{
    // Do all the test suites
    std::cout << "TEST SUITES FOR CLASS BSTree" << std::endl;
    test_class_BSTree_types(t);
    test_class_BSTree_very_small(t);
    test_class_BSTree_larger(t);
    test_class_BSTree_copy_ctor(t);
    test_class_BSTree_copy_assn(t);
}


// ************************************************************************
// Main program
// ************************************************************************


// main
// Runs class BSTree test suite, prints results to cout.
int main()
{
    Tester t;
    test_class_BSTree(t);

    std::cout << std::endl;
    if (t.allPassed())
    {
        std::cout << "All tests successful" 
                  << std::endl;
    }
    else
    {
        std::cout << "Tests ********** UNSUCCESSFUL **********"
                  << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Press ENTER to quit ";
    while (std::cin.get() != '\n') ;

    return 0;
}


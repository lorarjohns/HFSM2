# HFSM (Hierarchical Finite State Machine) Framework [![Build Status](https://travis-ci.org/andrew-gresyk/HFSM.svg?branch=master)](https://travis-ci.org/andrew-gresyk/HFSM)

Header-only heriarchical FSM framework in C++14, completely static (no dynamic allocations), built with variadic templates.

## Compiler Support

- Visual Studio 14.u3, 15.7
- GCC 4.9, 5.4, 6.3, 7.3, 8.0
- Clang 3.6, 3.7, 3.8, 3.9, 4.0, 5.0, 6.0

---

## Basic Usage

```cpp
// 1. Include HFSM header:
#include <hfsm/machine_single.hpp>

// 2. Define interface class between the FSM and its owner
//    (also ok to use the owner object itself):
struct Context { /* ... */ };

// 3. (Optional) Typedef hfsm::Machine for convenience:
using M = hfsm::Machine<OwnerClass>;

// 4. Define states:
struct MyState1 : M::Bare {
    // 5. Override some of the following state functions:
    void enter(Context& _);
    void update(Context& _);
    void transition(Control& c, Context& _);
    void leave(Context& _);
};

struct MyState2 : M::Bare { /* .. */ };
struct MySubState1 : M::Bare { /* .. */ };
struct MySubState2 : M::Bare { /* .. */ };

struct MyState3 : M::Bare { /* .. */ };
struct MyOrthogonalState1 : M::Bare { /* .. */ };
struct MyOrthogonalState2 : M::Bare { /* .. */ };

// 6. Declare state machine structure:
using MyFSM = M::PeerRoot<
    MyState1,
    M::Composite<MyState2,
        MySubState1,
        MySubState2,
    >,
    M::Orthogonal<MyState3,
        MyOrthogonalState1,
        MyOrthogonalState2,
    >
>;

int main() {
    // 7. Create context and state machine instances:
    Context context;
    MyFSM fsm(context);

    // 8. Kick off periodic updates:
    bool running = true;
    while (running)
        fsm.update();

    return 0;
}
```

---

## Feature Highlights

- Permissive [MIT License](LICENSE.md)
- Written in widely-supported modern(ish) C++ 14
- 100% NoUML-compliant
- Not hamstrung by restrictive event reaction-based approach, but supports powerful event handling
- Hierarchical, with composite (sub-machine) and orthogonal regions
- Header-only
- Fully static, no dynamic allocations
- Uses inline-friendly compile-time pylymorphism, no virtual methods were harmed
- Type-safe transitions: `FSM.changeTo<TargetState>()`
- Gamedev-friendly, supports explicit `State::update()`
- Scaleable, supports state re-use via state injections
- Debug-assisted, includes automatic structure and activity visualization API with `#define HFSM_ENABLE_STRUCTURE_REPORT`
- Convenient, minimal boilerplate

---

## Documentation

- [Tutorial](doc/tutorial.md)

### Design

- Another FSM lib?
- NoUML compliance
- Proactive vs. reactive approach
- Gamedev requirements
- Alternatives

### In-Depth

- Context and M:: 'namespace'
- Basic state methods
- Basic transitions
- Roots and regions
- Transitions within hierarchy
- Active chain
- Quering state activation status
- Substitutions, aka State guards on steroids
- Event handling
- State reuse with injections
- Structure and activity report API
- Assisted debugging with custom .natvis

---

## Get Updates

- [blog](https://andrew-gresyk.github.io/)
- [twitter](https://www.twitter.com/andrew_gresyk)

---

## Special Thanks

- [Phil Nash](https://github.com/philsquared)
- [Romain Cheminade](https://github.com/romaincheminade)
- [Tristan Brindle](https://github.com/tcbrindle)
- [Kevin Greene](https://github.com/kgreenek)
- everybody at [C++::London](https://www.meetup.com/CppLondon/) meetup
- programming community at [Splash Damage](http://www.splashdamage.com/)
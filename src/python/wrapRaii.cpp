// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/ts/raii.h>

#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/operators.hpp>
#include <pxr/boost/python/return_arg.hpp>
#include <memory>

using namespace pxr;

using namespace pxr::boost::python;


namespace
{
    class _PyAntiRegressionSelector
    {
    public:
        _PyAntiRegressionSelector(TsAntiRegressionMode mode)
            : _mode(mode)
        {
        }

        void Enter()
        {
            _selector.reset(new TsAntiRegressionAuthoringSelector(_mode));
        }

        void Exit(const object &, const object &, const object &)
        {
            _selector.reset();
        }

    private:
        const TsAntiRegressionMode _mode;
        std::unique_ptr<TsAntiRegressionAuthoringSelector> _selector;
    };

    class _PyBehaviorBlock
    {
    public:
        void Enter()
        {
            _block.reset(new TsEditBehaviorBlock());
        }

        void Exit()
        {
            _block.reset();
        }

    private:
        std::unique_ptr<TsEditBehaviorBlock> _block;
    };
}


void wrapRaii()
{
    // Context-manager class that temporarily sets the current anti-regression
    // authoring mode for the calling thread.  Use in a 'with' statement.
    class_<_PyAntiRegressionSelector, noncopyable>(
        "AntiRegressionAuthoringSelector", no_init)
        .def(init<TsAntiRegressionMode>())
        .def("__enter__", &_PyAntiRegressionSelector::Enter, return_self<>())
        .def("__exit__", &_PyAntiRegressionSelector::Exit)
        ;

    // Context-manager class that temporarily prevents automatic behaviors when
    // editing splines.  Use in a 'with' statement.
    class_<_PyBehaviorBlock, noncopyable>(
        "EditBehaviorBlock")
        // Default init not suppressed, so automatically created.
        .def("__enter__", &_PyBehaviorBlock::Enter, return_self<>())
        .def("__exit__", &_PyBehaviorBlock::Exit)
        ;
}

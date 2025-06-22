# Copyright 2023 Pixar
#
# Licensed under the terms set forth in the LICENSE.txt file available at
# https://openusd.org/license.
#
# Modified by Jeremy Retailleau.

from ts_test import TsTest_Museum as Museum
from ts_test import TsTest_Baseliner as Baseliner
from ts_test import TsTest_SampleTimes as STimes
from ts_test import TsTest_Grapher as Grapher
from ts_test import TsTest_Comparator as Comparator

try:
    from ts_test import TsTest_AnimXEvaluator as Evaluator
except ImportError:
    Evaluator = None

import os
import unittest
import pytest

@pytest.mark.skipif(Evaluator is None, reason="animx tests not available")
class TsTest_AnimXFramework(unittest.TestCase):

    def test_Grapher(self):
        """
        Verify that AnimXEvaluator and Grapher are working.
        To really be sure, inspect the graph image output.
        """
        data1 = Museum.GetData(Museum.TwoKnotBezier)
        data2 = Museum.GetData(Museum.TwoKnotLinear)

        times = STimes(data1)
        times.AddStandardTimes()

        samples1 = Evaluator().Eval(data1, times)
        samples2 = Evaluator().Eval(data2, times)

        grapher = Grapher("test_Grapher")
        grapher.AddSpline("Bezier", data1, samples1)
        grapher.AddSpline("Linear", data2, samples2)

        if Grapher.Init():
            grapher.Write("test_Grapher.png")

    def test_Comparator(self):
        """
        Verify that AnimXEvaluator and Comparator are working.
        To really be sure, inspect the graph image output.
        """
        data1 = Museum.GetData(Museum.TwoKnotBezier)
        data2 = Museum.GetData(Museum.TwoKnotLinear)

        times = STimes(data1)
        times.AddStandardTimes()

        samples1 = Evaluator().Eval(data1, times)
        samples2 = Evaluator().Eval(data2, times)

        comparator = Comparator("test_Comparator")
        comparator.AddSpline("Bezier", data1, samples1)
        comparator.AddSpline("Linear", data2, samples2)

        if Comparator.Init():
            comparator.Write("test_Comparator.png")

        self.assertTrue(comparator.GetMaxDiff() < 1.0)

    def test_Baseline(self):
        """
        Verify that AnimXEvaluator and Baseliner are working.
        """
        data = Museum.GetData(Museum.TwoKnotBezier)

        times = STimes(data)
        times.AddStandardTimes()

        samples = Evaluator().Eval(data, times)

        baseliner = Baseliner.CreateForEvalCompare(
            os.environ["TS_ANIMX_FRAMEWORK_PATH"],
            "test_Baseline", data, samples)
        self.assertTrue(baseliner.Validate())


if __name__ == "__main__":
    unittest.main()

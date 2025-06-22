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
    from ts_test import TsTest_MayapyEvaluator as Evaluator

    # MayapyEvaluator subclass that writes debug messages to stdout.
    class _TestEvaluator(Evaluator):
        def _DebugLog(self, msg):
            sys.stdout.write(msg)

except ImportError:
    Evaluator = None

import sys, unittest
import pytest

g_evaluator = None


@pytest.mark.skipif(Evaluator is None, reason="mayapy tests not available")
class TsTest_MayapyFramework(unittest.TestCase):

    def test_Grapher(self):
        """
        Verify that MayapyEvaluator and Grapher are working.
        To really be sure, inspect the graph image output.
        """
        data1 = Museum.GetData(Museum.TwoKnotBezier)
        data2 = Museum.GetData(Museum.TwoKnotLinear)

        times = STimes(data1)
        times.AddStandardTimes()

        samples1 = g_evaluator.Eval(data1, times)
        samples2 = g_evaluator.Eval(data2, times)

        grapher = Grapher("test_Grapher")
        grapher.AddSpline("Bezier", data1, samples1)
        grapher.AddSpline("Linear", data2, samples2)

        if Grapher.Init():
            grapher.Write("test_Grapher.png")

    def test_Comparator(self):
        """
        Verify that MayapyEvaluator and Comparator are working.
        To really be sure, inspect the graph image output.
        """
        data1 = Museum.GetData(Museum.TwoKnotBezier)
        data2 = Museum.GetData(Museum.TwoKnotLinear)

        times = STimes(data1)
        times.AddStandardTimes()

        samples1 = g_evaluator.Eval(data1, times)
        samples2 = g_evaluator.Eval(data2, times)

        comparator = Comparator("test_Comparator")
        comparator.AddSpline("Bezier", data1, samples1)
        comparator.AddSpline("Linear", data2, samples2)

        if Comparator.Init():
            comparator.Write("test_Comparator.png")

        self.assertTrue(comparator.GetMaxDiff() < 1.0)

    def test_Baseline(self):
        """
        Verify that MayapyEvaluator and Baseliner are working.
        """
        data = Museum.GetData(Museum.TwoKnotBezier)

        times = STimes(data)
        times.AddStandardTimes()

        samples = g_evaluator.Eval(data, times)

        baseliner = Baseliner.CreateForEvalCompare(
            "test_Baseline", data, samples)
        self.assertTrue(baseliner.Validate())

    @classmethod
    def tearDownClass(cls):
        """
        Clean up after all tests have run.
        """
        g_evaluator.Shutdown()


if __name__ == "__main__":

    mayapyPath = sys.argv.pop()
    g_evaluator = _TestEvaluator(
        mayapyPath, subprocessDebugFilePath = "debugMayapyDriver.txt")

    unittest.main()

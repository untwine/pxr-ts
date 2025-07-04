# Copyright 2023 Pixar
#
# Licensed under the terms set forth in the LICENSE.txt file available at
# https://openusd.org/license.
#
# Modified by Jeremy Retailleau.

from ts_test import TsTest_Museum as Museum
from ts_test import TsTest_SampleTimes as STimes
from ts_test import TsTest_Comparator as Comparator
from ts_test import TsTest_SampleBezier as SampleBezier

try:
    from ts_test import TsTest_MayapyEvaluator as MayapyEvaluator
except ImportError:
    MayapyEvaluator = None

try:
    from ts_test import TsTest_AnimXEvaluator as AnimXEvaluator
except ImportError:
    AnimXEvaluator = None

import sys, unittest
import pytest

g_mayapyEvaluator = None


@pytest.mark.skipif(
    MayapyEvaluator is None or AnimXEvaluator is None,
    reason="mayapy or/and animx tests not available"
)
class TsTest_MayapyVsAnimX(unittest.TestCase):

    def _DoTest(self, case, dataId, tolerance, addRaw = False):
        """
        Verify mayapy and AnimX evaluation are close.
        To really be sure, inspect the graph image output.

        TODO: can these two backends match exactly?
        """
        data = Museum.GetData(dataId)

        times = STimes(data)
        times.AddStandardTimes()

        mayapySamples = g_mayapyEvaluator.Eval(data, times)
        animXSamples = AnimXEvaluator().Eval(data, times)

        comparator = Comparator(case)
        comparator.AddSpline("mayapy", data, mayapySamples)
        comparator.AddSpline("AnimX", data, animXSamples)

        if addRaw:
            checkSamples = SampleBezier(data, numSamples = 200)
            comparator.AddSpline("Raw Bezier", data, checkSamples)

        if Comparator.Init():
            comparator.Write(f"{case}.png")

        self.assertTrue(comparator.GetMaxDiff() < tolerance)

    def test_Basic(self):
        self._DoTest("test_Basic", Museum.TwoKnotBezier, 1e-7)

    def test_RegressiveLoop(self):
        self._DoTest("test_RegressiveLoop", Museum.RegressiveLoop,
                     1e-5, addRaw = True)

    def test_BoldS(self):
        self._DoTest("test_BoldS", Museum.BoldS, 1e-5)

    def test_Cusp(self):
        self._DoTest("test_Cusp", Museum.Cusp, 1e-6)

    @classmethod
    def tearDownClass(cls):
        """
        Clean up after all tests have run.
        """
        g_mayapyEvaluator.Shutdown()


if __name__ == "__main__":

    mayapyPath = sys.argv.pop()
    g_mayapyEvaluator = MayapyEvaluator(mayapyPath)

    unittest.main()

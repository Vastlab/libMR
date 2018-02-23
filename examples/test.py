import libmr
import unittest
import numpy as np

class TestMR(unittest.TestCase):
    def test_str(self):
        # We should be able to convert an MR to a string
        mr = libmr.MR()
        mr.fit_high(np.random.randn(100), 100)
        assert len(str(mr)) > 0

    def test_load_from_string(self):
        # We should be able to deserialize a string to an MR
        mr = libmr.MR()
        mr.fit_high(np.random.randn(100), 100)
        saved = str(mr)
        mr2 = libmr.load_from_string(saved)
        assert str(mr) == str(mr2)

if __name__ == '__main__':
    unittest.main()

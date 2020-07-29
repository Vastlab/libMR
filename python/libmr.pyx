#
# libmr.pyx:
#
# @Author Terry Boult tboult at securics com
# @Author Vijay Iyer viyer at securics com
# @Author Michael Wilber mwilber at securics.com
#
# Copyright 2013, Securics Inc.
#
#   See accompanying LICENSE agrement for details on rights.
#
# Parts of this technology are subject to SBIR data rights and as
# described in DFARS 252.227-7018 (June 1995) SBIR Data Rights which
# apply to Contract Number: N00014-11-C-0243 and STTR N00014-07-M-0421
# to Securics Inc, 1870 Austin Bluffs Parkway, Colorado Springs, CO
# 80918
#
# The Government's rights to use, modify, reproduce, release, perform,
# display, or disclose technical data or computer software marked with
# this legend are restricted during the period shown as provided in
# paragraph (b)(4) of the Rights in Noncommercial Technical Data and
# Computer Software-Small Business Innovative Research (SBIR) Program
# clause contained in the above identified contract. Expiration of
# SBIR Data Rights: Expires four years after completion of the above
# cited project work for this or any other follow-on SBIR contract,
# whichever is later.
#
# No restrictions on government use apply after the expiration date
# shown above. Any reproduction of technical data, computer software,
# or portions thereof marked with this legend must also reproduce the
# markings.

from libc.stdlib cimport malloc,free
from libcpp cimport bool
from libcpp.string cimport string
cimport numpy as np
import numpy as np
import sys

cdef extern from "MetaRecognition.h":
    enum MR_fitting_type:
        complement_reject=1
        positive_reject=2
        complement_model=3
        positive_model=4
    cppclass MetaRecognition:
        MetaRecognition(int scores_to_drop,
                        int fitting_size,
                        bool verbose,
                        double alpha,
                        int translate_amount) except +
        bool is_valid()
        void set_translate(double t)
        void Reset()
        bool Predict_Match(double x, double threshold)
        double W_score(double x)
        double PDF(double x)
        double CDF(double x)
        double Inv(double p)

        int ReNormalize(double *invec, double *outvec, int length)
        int ReNormalizePDF(double *invec, double *outvec, int length)
        int FitHigh(double* inputData, int inputDataSize,  int fit_size)

        int FitLow(double* inputData, int inputDataSize,  int fit_size)


        # void Save(FILE *outputFile) const
        # void Load(FILE *inputFile)
        void Save(char* filename)
        void Load(char* filename)
        int get_fitting_size()
        int set_fitting_size(int nsize)
        int get_translate_amount()
        int set_translate_amount(int ntrans)
        int get_sign()
        int set_sign(int nsign)
        double get_small_score()
        double set_small_score(double nscore)
        bool verbose
        string to_string()
        void from_string(string input)
        double get_scale_param()
        double get_shape_param()
        void set_scale_param(double scale)
        void set_shape_param(double shape)
        double get_scale_confidence_lb()
        double get_scale_confidence_ub()
        double get_shape_confidence_lb()
        double get_shape_confidence_ub()
        void set_scale_confidence_lb(double val)
        void set_scale_confidence_ub(double val)
        void set_shape_confidence_lb(double val)
        void set_shape_confidence_ub(double val)
        bool set_valid()
        bool set_invalid()
        unsigned binary_size()
        void to_binary(unsigned char* data)
        void from_binary(const unsigned char* data)



# This is the Python wrapper class.
cdef class MR:
    cdef MetaRecognition *thisptr
    def __cinit__(self, int scores_to_drop=0,
                  int fitting_size=9,
                  bool verbose=False,
                  double alpha=5.0,
                  int translate_amount=1):
        """
        Create a new MR object.
        """
        self.thisptr = new MetaRecognition(scores_to_drop,fitting_size,verbose,alpha,translate_amount)
    def __dealloc__(self):
        del self.thisptr
    def fit_low(self, inputData, int fit_size):
        """Use fit_low if your data is such that is smaller is better. Fits a
        MR object to the given data. We'll transform it for you
        and keep the transform parameters in the class so later calls
        to W_score or CDF do the right thing."""
        if len(inputData) < fit_size:
          raise ValueError("The given fit_size %d is larger than the data size %d" % (fit_size, len(inputData)))
        cdef double *data
        data = <double*>malloc(sizeof(double)*len(inputData))
        for i in xrange(len(inputData)):
            data[i] = inputData[i]
        self.thisptr.FitLow(data, len(inputData), fit_size)
        free(data)
    def fit_high(self, inputData, int fit_size):
        """Use fit_high if your data is such that is larger is better. Fits a
        MR object to the given data. We'll transform it for you
        and keep the transform parameters in the class so later calls
        to W_score or CDF do the right thing.
        """
        if len(inputData) < fit_size:
          raise ValueError("The given fit_size %d is larger than the data size %d" % (fit_size, len(inputData)))
        cdef double *data
        data = <double*>malloc(sizeof(double)*len(inputData))
        for i in xrange(len(inputData)):
            data[i] = inputData[i]
        self.thisptr.FitHigh(data, len(inputData), fit_size)
        free(data)
    property is_valid:
        def __get__(self):
            return self.thisptr.is_valid()
    def get_params(self):
        """scale,shape,translate_amount,small_score"""
        return (self.thisptr.get_scale_param(),
                self.thisptr.get_shape_param(),
                self.thisptr.get_sign(),
                self.thisptr.get_translate_amount(),
                self.thisptr.get_small_score())


    def get_confidence(self):
        """scale_confidence_lb,scale_confidence_ub,shape_confidence_lb,shape_ub"""
        return (self.thisptr.get_scale_confidence_lb(),
                self.thisptr.get_scale_confidence_ub(),
                self.thisptr.get_shape_confidence_lb(),         
                self.thisptr.get_shape_confidence_ub()
                )


    def set_params(self,double scale, double shape, int sign, int translate_amount, double small_score):
        self.thisptr.set_scale_param(scale)
        self.thisptr.set_shape_param(shape)
        self.thisptr.set_sign(sign)
        self.thisptr.set_translate_amount(translate_amount)
        self.thisptr.set_small_score(small_score)

    def set_valid(self):
        return self.thisptr.set_valid()

    def set_invalid(self):
        return self.thisptr.set_invalid()

    def reset(self):
        self.thisptr.Reset()
    def predict_match(self, double x, double threshold = .9999999):
        """
        Is X from the "match" distribution (i.e. we reject null hypothesis
        of non-match)

        """
        return self.thisptr.Predict_Match(x,threshold)
    def w_score(self, double x):
        """
This is the commonly used function. After fitting, it returns the probability of the given score being "correct".  It is the same as CDF
        """
        return self.thisptr.W_score(x)

    def pdf(self,double x):
        return self.thisptr.PDF(x)

    def pdf_vector(self,double[::1] invec):
        cdef np.ndarray[np.double_t,ndim=1]new_vec = np.zeros(len(invec), dtype='d')
        self.thisptr.ReNormalizePDF(&invec[0], &new_vec[0], len(invec))
        return new_vec

    def cdf(self, double x):
        """
        This is the cummumlative probablity of match being corrrect (or more precisely the probility the score (after transform) being an outlier for the distribution, which given the transforms applied, so bigger is better, this is the probablity the score is correct.
        """
        return self.thisptr.CDF(x)
    def inv(self, double p):
        """
        This is score for which one would obtain CDF probability p (i.e. x such that p = CDF(x))
        """
        return self.thisptr.Inv(p)
    def w_score_vector(self, double[::1] invec):
        """
        Apply w_score to each element of invec, returning a new vector of W-scores
        """
        cdef np.ndarray[np.double_t,ndim=1]new_vec = np.zeros(len(invec), dtype='d')
        self.thisptr.ReNormalize(&invec[0], &new_vec[0], len(invec))
        return new_vec
    def __str__(self):
        """
        Serialize the MR object to a string. Use load_from_string to recover it.
        """
        if sys.version_info >= (3,):
            return str(self.thisptr.to_string(), 'utf-8')
        return self.thisptr.to_string()
    def __repr__(self):
        return "<MR object: %r>" % str(self)
    def as_binary(self):
        """Returns a binary representation of this class"""
        cdef unsigned char* data = <unsigned char*>malloc(self.thisptr.binary_size())
        self.thisptr.to_binary(data)
        cdef bytearray retval = data[:self.thisptr.binary_size()]
        free(data)
        return retval
    def __reduce__(self):
        """Makes the MR object picklable"""
        return (load_from_binary, (self.as_binary(),))

    property tailsize:
        def __get__(self):
            return self.thisptr.get_fitting_size()
        def __set__(self, int nsize):
            self.thisptr.set_fitting_size(nsize)
    property translate_amount:
        def __get__(self):
            return self.thisptr.get_translate_amount()
        def __set__(self, int ntrans):
            self.thisptr.set_translate_amount(ntrans)
    property sign:
        def __get__(self):
            return self.thisptr.get_sign()
        def __set__(self, int nsign):
            self.thisptr.set_sign(nsign)
    property small_score:
        def __get__(self):
            return self.thisptr.get_small_score()
        def __set__(self, double nscore):
            self.thisptr.set_small_score(nscore)
    property verbose:
        def __get__(self):
            return self.thisptr.verbose
        def __set__(self, bool verbose):
            self.thisptr.verbose = verbose

def load_from_string(str input):
    """
    Deserialize an MR object. This turns a string back into an MR object; it is the inverse of str(MR())
    """
    pymr = MR()
    pymr.thisptr.from_string(input.encode('utf-8'))
    return pymr

def load_from_binary(bytearray input):
    """Loads a MR object from the given binary representation and returns it."""
    cdef unsigned char* data = input
    pymr = MR()
    pymr.thisptr.from_binary(data)
    return pymr

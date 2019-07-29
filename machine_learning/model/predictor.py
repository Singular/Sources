"""
Define the predictor class for classifying according to help page.
"""

# import cProfile
import os
import sys
import time

# Third party imports
import numpy as np
from sklearn.base import BaseEstimator, ClassifierMixin

# Local imports
from common.keyword_vector import vector_distance, count_occurances, \
        read_dictionary, normalise_vector
from common.lookuptable import create_table
from common.constants import KEYWORDS_FILE


class HelpPagePredictor(BaseEstimator, ClassifierMixin):
    """
    Classifier to assign the given input to a Singular helppage.
    """
    def __init__(self):
        """
        Define attributes
        """
        self.vectors = None
        self.files = None


    def fit(self, X, y): # pylint: disable=invalid-name
        """
        Setup the correspondence of vectors to help-files
        """
        assert X is not None, "Please provide data for X"
        assert y is not None, "Please provide data for y"
        self.vectors = X
        self.files = y
        return self


    def predict(self, X): # pylint: disable=invalid-name
        """
        Classify the input vectors
        """
        assert X is not None, "Please provide data for X"
        ret_list = []
        for x in X: # pylint: disable=invalid-name
            # find the closest vector
            min_val = float("inf")
            index = -1
            i = 0
            for vec in self.vectors:
                # dist = vector_distance(x, vec)
                # Dot product is much faster
                dist = -1 * np.dot(x, vec)
                if dist < min_val:
                    min_val = dist
                    index = i
                i = i + 1

            # find corresponding filename
            file = self.files[index]
            ret_list.append(file)
        return np.array(ret_list)


def basic_vector_tests():
    """
    Some basic sanity tests
    """
    predictor = HelpPagePredictor()
    vector1 = normalise_vector(np.array([1, 4, 10]))
    vector2 = normalise_vector(np.array([2, 3, 1]))
    vector3 = normalise_vector(np.array([3, 9, 3]))

    vectors = np.array([vector1, vector2, vector3])
    files = np.array(["file1", "file2", "file3"])
    print(vectors)
    print(files)
    print()

    testvec = normalise_vector(np.array([1, 1, 1]))
    print("test vector:")
    print(testvec)
    print()

    print("distance to 1")
    print(vector_distance(testvec, vector1))
    print()
    print("distance to 2")
    print(vector_distance(testvec, vector2))
    print()
    print("distance to 3")
    print(vector_distance(testvec, vector3))
    print()

    predictor.fit(vectors, files)
    prediction = predictor.predict(np.array([testvec]))
    print("Prediction:")
    print(prediction)
    print()


def main():
    """
    Run some basic tests
    """
    print("Running some tests")

    basic_vector_tests()

    dictionary = read_dictionary(KEYWORDS_FILE)

    start = time.time()
    vectors, file_list = create_table(dictionary=dictionary)
    end = time.time()
    print(end - start, "seconds to create_table")

    test_vec = count_occurances("extract.lib", dictionary)
    predictor = HelpPagePredictor()
    predictor.fit(vectors, file_list)

    start = time.time()
    prediction = predictor.predict(np.array([test_vec]))
    end = time.time()
    print(end - start, "seconds to make prediction")
    print(prediction)
    print()

    print("prediction for zero vector")
    zerovec = np.zeros(len(dictionary) - 2)
    start = time.time()
    prediction = predictor.predict(np.array([zerovec]))
    end = time.time()
    print(end - start, "seconds to make prediction")
    print(prediction)
    print()

    if len(sys.argv) >= 2:
        for i in range(len(sys.argv)):
            if i == 0:
                continue
            if not os.path.isfile(sys.argv[i]):
                continue
            print("predicting for file", sys.argv[i])
            test_vec = count_occurances(sys.argv[i], dictionary)
            start = time.time()
            prediction = predictor.predict(np.array([test_vec]))
            end = time.time()
            print(end - start, "seconds to make prediction")
            print(prediction)
            print()


if __name__ == '__main__':
    #cProfile.run("main()")
    main()
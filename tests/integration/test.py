#!/usr/bin/python3

from flagit import FlagIt
from flagit import DataFetcher

CUSTOM_PYTHON_URL = "http://python_fetcher.com"

class DataFetcherPython(DataFetcher):
    def __init__(self, url):
        DataFetcher.__init__(self, url)
        self.url = url

    def fetch_data(self, url):
        assert CUSTOM_PYTHON_URL == url
        return '''{"python_feature1": { "enabled": true,
                               "enabled_for" : ["robert", "john", "ricky"],
                               "disabled_for" : ["andrea", "kelly"]
                    }
                }'''

flag_obj = FlagIt('http://127.0.0.1:9000/wowza/sample.json')

assert flag_obj.enabled_for('feature1', 'john')
assert flag_obj.enabled_for('feature1', 'alex') is False
assert flag_obj.disabled_for('feature1', 'john') is False
assert flag_obj.disabled_for('feature1', 'alex') is False
assert flag_obj.disabled_for('feature1', 'kelly')
assert flag_obj.disabled_for('feature1', 'andrea')

assert flag_obj.enabled('feature1')
assert flag_obj.enabled('feature2') is False
print(flag_obj.enabled("feature1"))

try:
    print(flag_obj.enabled_for('feature2', 'alex'))
except RuntimeError as e:
    print(e)

try:
    FlagIt("http://127.0.0.1:9000/wowza/sample.json")
except ValueError as e:
    print(e)

pythonFlagIt = FlagIt(DataFetcherPython(CUSTOM_PYTHON_URL).__disown__())
assert pythonFlagIt.enabled_for('python_feature1', 'john')
assert pythonFlagIt.enabled_for('python_feature1', 'alex') is False
assert pythonFlagIt.disabled_for('python_feature1', 'john') is False
assert pythonFlagIt.enabled_for('python_feature1', 'kelly') is False
assert pythonFlagIt.enabled('python_feature1')

# TODO: Figure out how to avoid this.
del pythonFlagIt

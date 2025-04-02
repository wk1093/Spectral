from json import JSONEncoder

class MyEncoder(JSONEncoder):
    def default(self, o):
        if isinstance(o, set):
            return list(o)
        else :
            return o.__dict__

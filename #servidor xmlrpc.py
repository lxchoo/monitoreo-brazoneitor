#servidor rpc
from xmlrpc.server import SimpleXMLRPCServer

def add(a):
    return a*100

def main():
    print("Este es un servidor de procedimientos remotos")
    server = SimpleXMLRPCServer(("localhost", 5000))
    server.register_function(add)
    server.serve_forever()
    
if __name__=="__main__":
    main()
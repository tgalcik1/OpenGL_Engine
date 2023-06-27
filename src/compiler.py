import subprocess
from xml.etree.ElementInclude import include

file_name = "app"
include_dir = "dependencies/include"
lib_dir = "dependencies/lib"
lib = "-lglfw3 -lglew32 -lopengl32 -lglu32 -lgdi32"
preprocessor = "-DGLEW_STATIC"

def main():
    # compile
    subprocess.call(f"g++ -w -c GLapp.cpp -I {include_dir}")
    # link
    subprocess.call(f"g++ -w *.o -o {file_name} -L {lib_dir} {lib}")

if "__main__" == __name__:
    main()
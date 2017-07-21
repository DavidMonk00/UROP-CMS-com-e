# I2C Monitoring
## Installation Notes
### Prerequisites:
- Boost libraries:
    `yum install boost-devel`
- C++14 compatible compiler (GCC 5.3.1):
    * `sudo yum install centos-release-scl`
    * `sudo yum install devtoolset-4-gcc*`
    * `scl enable devtoolset-4 bash`
- SEMA libraries:
    1. Download from http://www.adlinktech.com/PD/web/PD_Driver.php?cp=Driver&PDNo=1274&kind=UT
    2. Run LINUX_x64 within binaries within archive
    3. Copy header files to /opt/Sema/include
    4. Link binaries from /usr/local/SEMA/lib -> /opt/Sema/lib
    5. Create header file "Sema.h" to collect includes
- pybind11 libraries:
    1. Install developer extensions for python:
        * `pip install pytest`
        * `yum install python-devel`
    2. Clone repository `git clone https://github.com/pybind/pybind11.git` into /opt
    3. Check installation:
        * `cd /opt/pybind11`
        * `mkdir build`
        * `cd build`
        * `cmake ..` (install not installed)
        * `make check -j 4`
- Crontab:
    * `crontab -e`:
        * `* * * * * /path-to-folder/bin/main.py`
        * `0 * * * * /path-to-folder/bin/purge.py`

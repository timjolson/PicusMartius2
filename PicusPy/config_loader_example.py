import configparser

# create parser and file path
config = configparser.ConfigParser()
filename = './picus.config'
fileexist = False

# try to read file, if not exist, returns empty
try:
    settings = config.read(filename)
except configparser.NoSectionError:
    print('no section')
except configparser.NoOptionError:
    print('no option')

try:
    print('file length: {}'.format(len(settings)))
    if not len(settings):
        print('no file: '+filename)
        print('creating file: '+filename)
        with open(filename, 'w') as f:
            config.write(f)
            fileexist = True
            f.close()
except configparser.Error as err:
    print(err)
    pass

if fileexist:
    with open(filename,'w') as file:
        try:
            config.add_section('PID Values')
        except configparser.DuplicateSectionError:
            pass

        config.set('PID Values', 'kP', str(.01))
        config.set('PID Values', 'kI', str(.02))
        config.set('PID Values', 'kD', str(.03))
        config.write(file)
        file.close()

try:
    config.read(filename)
    kP = config.getfloat('PID Values', 'kP')
    kI = config.getfloat('PID Values', 'kI')
    kD = config.getfloat('PID Values', 'kD')
    # config.getint()
    # config.
    print('kP= {}'.format(kP))
except:
    pass

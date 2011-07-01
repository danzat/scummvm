# .blb file unpacker
# Written by Onkel Hotte

import struct, array, wave, sys, os
from ctypes import *
from sys import argv

# Enable headless pygame

# set SDL to use the dummy NULL video driver,
#   so it doesn't need a windowing system.
os.environ["SDL_VIDEODRIVER"] = "dummy"

if True:
    # Some platforms might need to init the display for some parts of pygame.
    import pygame.display
    pygame.display.init()
    screen = pygame.display.set_mode((1,1))

import pygame
from pygame.locals import *

# Some magic to make the blast decompression function available from inside python

class BUFFER(Structure):
    _fields_ = [("data", c_void_p), ("len", c_uint)]

# typedef unsigned (*blast_in)(void *how, unsigned char **buf);
BLASTINFUNC = CFUNCTYPE(c_uint, c_void_p, POINTER(c_void_p))

def py_blast_in_func(a, b):
    buf = cast(a, POINTER(BUFFER))
    b[0] = buf.contents.data
    return buf.contents.len

blast_in_func = BLASTINFUNC(py_blast_in_func)

# typedef int (*blast_out)(void *how, unsigned char *buf, unsigned len);
BLASTOUTFUNC = CFUNCTYPE(c_int, c_void_p, c_void_p, c_uint)

def py_blast_out_func(a, b, c):
    buf = cast(a, POINTER(BUFFER))
    memmove(buf.contents.data + buf.contents.len, b, c)
    buf.contents.len += c
    return 0

blast_out_func = BLASTOUTFUNC(py_blast_out_func)

if __name__ == "__main__":
    if len(argv) != 2:
        print "Error: wrong number of arguments"
        sys.exit(0)
    # make an output directory
    output_dir = "%s.d" % argv[1]
    os.mkdir(output_dir)

    # You need to make a dynamic library of zlib/contrib/blast.c, on MacOS that is
    # gcc -dynamiclib -o blast.dylib -dylib blast.c

    lib_blast = CDLL("./blast.so")
    blast = lib_blast.blast
    blast.restype = c_int
    blast.argtypes = [BLASTINFUNC, c_void_p, BLASTOUTFUNC, c_void_p]

    f = open(argv[1], "rb")

    # Header
    (magic, id, unknown, dataSize, fileSize, file_number) = struct.unpack('<4sBBHLL', f.read(16))

    if magic != "\x40\x49\x00\x02":
        print "wrong id, not a blb file"
        exit()

    # file ID
    id_table = []
    for i in xrange(file_number):
        (id,) = struct.unpack('<L', f.read(4))
        id_table.append(id)

    # dir entries
    file_table = []
    for i in xrange(file_number):
        file_entry = struct.unpack('<BBHLLLL', f.read(20))
        file_table.append(file_entry)

    # offset shift table (for music and sfx files)
    shift_table = f.read(dataSize)

    # initial pal
    pal = [(x,x,x) for x in xrange(256)]

    # export files into raw
    file_type = { 7: "sound", 8: "music", 10: "video", 2:"image", 4:"animation" }
    file_ext = { 7: "wav", 8: "wav", 10: "smk", 2:"tga", 4:"tga" }
    for i in xrange(file_number):
        (type, mode, index, id, start, in_len, out_len) = file_table[i]
        if mode != 101:
            f.seek(start)

            data = f.read(in_len)
            if mode == 3:
                buffer = create_string_buffer(out_len)
                in_buf = BUFFER(cast(data, c_void_p), in_len)
                out_buf = BUFFER(cast(buffer, c_void_p), 0)
                blast(blast_in_func, byref(in_buf), blast_out_func, byref(out_buf))           

                data = string_at(buffer, out_len)

            if type in [7,8]:
                # sound/music
                if index > len(shift_table):
                    shift = 255
                else:
                    shift = ord(shift_table[index])

                w = wave.open("%s/%s-%i.%s" % (output_dir, file_type[type], i, file_ext[type]), "wb")
                w.setnchannels( 1 )
                w.setsampwidth( 2 )
                w.setframerate( 22050 )

                if shift < 16:
                    buffer = array.array( "h" )

                    curValue = 0
                    for val in data:
                        val = (ord(val) ^ 128) - 128
                        curValue += val
                        try:
                            buffer.append(curValue << shift)
                        except OverflowError:
                            try:
                                # This obviously means that something is going wrong with the decompression or with the scaling
                                buffer.append(curValue)
                            except OverflowError:
                                buffer.append(0)

                    # I'm not sure if this is required, it doesn't seem to be on MacOS. Maybe WAV does the endian-swap internally, or stores a flag in the wav header?
                    if sys.byteorder == "little":
                        buffer.byteswap()
                    w.writeframes( buffer.tostring() )
                    del buffer
                else:
                    if (len(data) & 1) != 0:
                        data = data + "\0"
                    w.writeframes( data )
                w.close()

            elif type == 2:
                #  image
                (format, width, height) = struct.unpack('<HHH', data[0:6])

                # Format flags
                # 0x1 - compressed
                # 0x4 - offset
                # 0x8 - palette

                pos = 6
                if format & 0x8:
                    pal = [(ord(x[0]), ord(x[1]), ord(x[2])) for x in [data[pos + idx * 4: pos + 3 + idx * 4] for idx in xrange(256)]]
                    pos += 1024
                    # I assume the palette is decided by the room an item appears in. But reusing the last palette makes things at least visible

                if format & 0x4:
                    # some kind of position?
                    offset = struct.unpack('<HH', data[pos:pos+4])
                    # print offset
                    pos += 4

                if format & 0x1:
                    # compressed
                    framebuffer = array.array("B")
                    framebuffer.extend([0 for x in xrange(width * height)])
                    ypos = 0
                    while True:
                        header = struct.unpack('<HH', data[pos:pos+4])
                        pos += 4
                        if header == (0,0):
                            break
                        (row_count, items_per_row_count) = header
                        for row_index in xrange(row_count):
                            for item_index in xrange(items_per_row_count):
                                (xpos, ) = struct.unpack('<H', data[pos:pos+2])
                                pos += 2
                                (fragment_len,) = struct.unpack('<H', data[pos:pos+2])
                                pos += 2
                                for b in xrange(fragment_len):
                                    framebuffer[ypos + xpos + b] = ord(data[pos + b])
                                pos += fragment_len
                            ypos += width

                    surface = pygame.image.fromstring(framebuffer.tostring(), (width, height), "P")
                    surface.set_palette(pal)
                    pygame.image.save(surface, "%s/%s-%i.%s" % (output_dir, file_type[type], i, file_ext[type]))
                    del framebuffer
                else:
                    # uncompressed
                    surface = pygame.image.fromstring(data[pos:pos + width * height], (width, height), "P")
                    surface.set_palette(pal)
                    pygame.image.save(surface, "%s/%s-%i.%s" % (output_dir, file_type[type], i, file_ext[type]))

            elif type == 4:
                header = struct.unpack('<HHIIII', data[0:20])
                # print "%x" % i, header

                palette_offset = header[3]
                pal = [(ord(x[0]), ord(x[1]), ord(x[2])) for x in [data[palette_offset + idx * 4: palette_offset + 3 + idx * 4] for idx in xrange(256)]]

                pos = 20
                if header[0] == 2:
                    unknown_header = struct.unpack('<HHHH', data[pos:pos+8])
                    # print unknown_header
                    pos += 8

                frame_count = header[5]
                frame_list = []
                for frame_index in xrange(frame_count):
                    frame = struct.unpack('<hhhhhhhhhhhhhhI', data[pos + frame_index * 32:pos + 32 + frame_index * 32])
                    # print frame
                    frame_list.append(frame)

                data_offset = header[2]
                for frame_index in xrange(frame_count):
                    frame = frame_list[frame_index]
                    width = frame[5]
                    height = frame[6]
                    pos = data_offset + frame[14]

                    framebuffer = array.array("B")
                    framebuffer.extend([0 for x in xrange(width * height)])
                    ypos = 0
                    while True:
                        header = struct.unpack('<HH', data[pos:pos+4])
                        pos += 4
                        if header == (0,0):
                            break
                        (row_count, items_per_row_count) = header
                        for row_index in xrange(row_count):
                            for item_index in xrange(items_per_row_count):
                                (xpos, ) = struct.unpack('<H', data[pos:pos+2])
                                pos += 2
                                (fragment_len,) = struct.unpack('<H', data[pos:pos+2])
                                pos += 2
                                for b in xrange(fragment_len):
                                    framebuffer[ypos + xpos + b] = ord(data[pos + b])
                                pos += fragment_len
                            ypos += width

                    surface = pygame.image.fromstring(framebuffer.tostring(), (width, height), "P")
                    surface.set_palette(pal)
                    pygame.image.save(surface, "%s/%s-%i-%i.%s" % (output_dir, file_type[type], i, frame_index, file_ext[type]))
                    del framebuffer
            else:
                of = open("%s/%s-%i.%s" % (output_dir, file_type[type], i, file_ext[type]), "wb")
                of.write(data)
                of.close()
            del data

    f.close() 

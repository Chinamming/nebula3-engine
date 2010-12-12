#pragma once
//------------------------------------------------------------------------------
/**
    @class IO::ZipFileStream
    
    Wraps a file in a zip archive into a stream. 
    The file int the zip-archive is not cached. Only forward reading is allowed.
    Only one file must be opened in that archive at a time.

    The IO::Server allows transparent access to data in zip files through
    normal "file:" URIs by first checking whether the file is part of
    a mounted zip archive. Only if this is not the case, the file will
    be opened as normal.
    
    To force reading from a zip archive, use an URI of the following 
    format:
    
    zip://[samba server]/bla/blob/archive.zip?file=path/in/zipfile&pwd=password

    This assumes that the URI scheme "zip" has been associated with
    the ZipFileStream class using the IO::Server::RegisterUriScheme()
    method.
    
    The server and local path part of the URI contain the path to
    the zip archive file. The query part contains the path of
    the file in the zip archive and an optional password.
    
    (C) 2006 Radon Labs GmbH
*/
#include "io/stream.h"

//------------------------------------------------------------------------------
namespace IO
{
class ZipFileEntry;
class ZipFileStream : public Stream
{
    __DeclareClass(ZipFileStream);
public:
    /// constructor
    ZipFileStream();
    /// destructor
    virtual ~ZipFileStream();
    /// memory streams support reading
    virtual bool CanRead() const;
    /// memory streams support writing
    virtual bool CanWrite() const;
    /// memory streams support seeking
    virtual bool CanSeek() const;
    /// memory streams are mappable
    virtual bool CanBeMapped() const;
    /// get the size of the stream in bytes
    virtual Size GetSize() const;
    /// get the current position of the read/write cursor
    virtual Position GetPosition() const;
    /// open the stream
    virtual bool Open();
    /// close the stream
    virtual void Close();
    /// directly read from the stream
    virtual Size Read(void* ptr, Size numBytes);
    /// seek in stream, only forward seeks are allowed
    virtual void Seek(Offset offset, SeekOrigin origin);
    /// return true if end-of-stream reached
    virtual bool Eof() const;
    /// map for direct memory-access
    virtual void* Map();
    /// unmap a mapped stream
    virtual void Unmap();

private:
    Size size;
    Position position;
    ZipFileEntry *zipFileEntry;
    unsigned char *mapBuffer;
};

} // namespace IO
//------------------------------------------------------------------------------

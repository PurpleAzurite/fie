fie is a simple and lightweight repalcement for ls written in C++. It prioritizes being fast and tiny over being feature rich.

![](https://i.imgur.com/8CGJwbo.png)

Usage: fie <optional: filesystem_path>

What's so good?
- Tiny binary ~168k
- C++ fast
- Single .cpp file, easy to expand and customize
- Very simple to add custom printout styles thanks to fmt

Todo: 
  - Show file owner
  - Fix bug when fetching last_write_time for entries that do not have it
  - Improve symlink entry appearance (show real path)
  - Have symlink directories display as symlink instead of dir

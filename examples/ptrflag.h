/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

struct foonode
{
  foonode *left, *right, *up, *down;
  static int nodecntr;
  int nodeid;
  foonode() {nodeid=nodecntr++;}
};


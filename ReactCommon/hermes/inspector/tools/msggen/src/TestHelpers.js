/**
 * Copyright 2004-present Facebook. All Rights Reserved.
 */
'use strict';

// munges string so that it's nice to look at in a test diff
function strip(str) {
  // Trim leading and trailing WS
  str = str.replace(/^\s+/, '');
  str = str.replace(/\s+$/, '');

  // Collapse all repeating newlines (possibly with spaces in between) into a
  // single newline
  str = str.replace(/\n(\s*)/g, '\n');

  // Collapse all non-newline whitespace into a single space
  return str.replace(/[^\S\n]+/g, ' ');
}

export function expectCodeIsEqual(actual, expected) {
  expect(strip(actual)).toBe(strip(expected));
}

export class FakeWritable {
  constructor() {
    this.result = '';
  }

  write(str) {
    this.result += str;
  }

  get() {
    return this.result;
  }
}

#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0

"""
This script helps track the translation status of the documentation
in different locales, e.g., zh_CN. More specially, it uses `git log`
commit to find the latest english commit from the translation commit
(order by author date) and the latest english commits from HEAD. If
differences occur, report the file and commits that need to be updated.

The usage is as follows:
- ./scripts/checktransupdate.py -l zh_CN
<<<<<<< HEAD
This will print all the files that need to be updated in the zh_CN locale.
=======
This will print all the files that need to be updated or translated in the zh_CN locale.
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
- ./scripts/checktransupdate.py Documentation/translations/zh_CN/dev-tools/testing-overview.rst
This will only print the status of the specified file.

The output is something like:
<<<<<<< HEAD
Documentation/translations/zh_CN/dev-tools/testing-overview.rst (1 commits)
commit 42fb9cfd5b18 ("Documentation: dev-tools: Add link to RV docs")
"""

import os
from argparse import ArgumentParser, BooleanOptionalAction
from datetime import datetime

flag_p_c = False
flag_p_uf = False
flag_debug = False


def dprint(*args, **kwargs):
    if flag_debug:
        print("[DEBUG] ", end="")
        print(*args, **kwargs)


def get_origin_path(file_path):
=======
Documentation/dev-tools/kfence.rst
No translation in the locale of zh_CN

Documentation/translations/zh_CN/dev-tools/testing-overview.rst
commit 42fb9cfd5b18 ("Documentation: dev-tools: Add link to RV docs")
1 commits needs resolving in total
"""

import os
import time
import logging
from argparse import ArgumentParser, ArgumentTypeError, BooleanOptionalAction
from datetime import datetime


def get_origin_path(file_path):
    """Get the origin path from the translation path"""
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
    paths = file_path.split("/")
    tidx = paths.index("translations")
    opaths = paths[:tidx]
    opaths += paths[tidx + 2 :]
    return "/".join(opaths)


def get_latest_commit_from(file_path, commit):
<<<<<<< HEAD
    command = "git log --pretty=format:%H%n%aD%n%cD%n%n%B {} -1 -- {}".format(
        commit, file_path
    )
    dprint(command)
=======
    """Get the latest commit from the specified commit for the specified file"""
    command = f"git log --pretty=format:%H%n%aD%n%cD%n%n%B {commit} -1 -- {file_path}"
    logging.debug(command)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
    pipe = os.popen(command)
    result = pipe.read()
    result = result.split("\n")
    if len(result) <= 1:
        return None

<<<<<<< HEAD
    dprint("Result: {}".format(result[0]))
=======
    logging.debug("Result: %s", result[0])
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

    return {
        "hash": result[0],
        "author_date": datetime.strptime(result[1], "%a, %d %b %Y %H:%M:%S %z"),
        "commit_date": datetime.strptime(result[2], "%a, %d %b %Y %H:%M:%S %z"),
        "message": result[4:],
    }


def get_origin_from_trans(origin_path, t_from_head):
<<<<<<< HEAD
=======
    """Get the latest origin commit from the translation commit"""
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
    o_from_t = get_latest_commit_from(origin_path, t_from_head["hash"])
    while o_from_t is not None and o_from_t["author_date"] > t_from_head["author_date"]:
        o_from_t = get_latest_commit_from(origin_path, o_from_t["hash"] + "^")
    if o_from_t is not None:
<<<<<<< HEAD
        dprint("tracked origin commit id: {}".format(o_from_t["hash"]))
=======
        logging.debug("tracked origin commit id: %s", o_from_t["hash"])
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
    return o_from_t


def get_commits_count_between(opath, commit1, commit2):
<<<<<<< HEAD
    command = "git log --pretty=format:%H {}...{} -- {}".format(commit1, commit2, opath)
    dprint(command)
=======
    """Get the commits count between two commits for the specified file"""
    command = f"git log --pretty=format:%H {commit1}...{commit2} -- {opath}"
    logging.debug(command)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
    pipe = os.popen(command)
    result = pipe.read().split("\n")
    # filter out empty lines
    result = list(filter(lambda x: x != "", result))
    return result


def pretty_output(commit):
<<<<<<< HEAD
    command = "git log --pretty='format:%h (\"%s\")' -1 {}".format(commit)
    dprint(command)
=======
    """Pretty print the commit message"""
    command = f"git log --pretty='format:%h (\"%s\")' -1 {commit}"
    logging.debug(command)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
    pipe = os.popen(command)
    return pipe.read()


<<<<<<< HEAD
def check_per_file(file_path):
    opath = get_origin_path(file_path)

    if not os.path.isfile(opath):
        dprint("Error: Cannot find the origin path for {}".format(file_path))
=======
def valid_commit(commit):
    """Check if the commit is valid or not"""
    msg = pretty_output(commit)
    return "Merge tag" not in msg

def check_per_file(file_path):
    """Check the translation status for the specified file"""
    opath = get_origin_path(file_path)

    if not os.path.isfile(opath):
        logging.error("Cannot find the origin path for {file_path}")
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
        return

    o_from_head = get_latest_commit_from(opath, "HEAD")
    t_from_head = get_latest_commit_from(file_path, "HEAD")

    if o_from_head is None or t_from_head is None:
<<<<<<< HEAD
        print("Error: Cannot find the latest commit for {}".format(file_path))
=======
        logging.error("Cannot find the latest commit for %s", file_path)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
        return

    o_from_t = get_origin_from_trans(opath, t_from_head)

    if o_from_t is None:
<<<<<<< HEAD
        print("Error: Cannot find the latest origin commit for {}".format(file_path))
        return

    if o_from_head["hash"] == o_from_t["hash"]:
        if flag_p_uf:
            print("No update needed for {}".format(file_path))
        return
    else:
        print("{}".format(file_path), end="\t")
        commits = get_commits_count_between(
            opath, o_from_t["hash"], o_from_head["hash"]
        )
        print("({} commits)".format(len(commits)))
        if flag_p_c:
            for commit in commits:
                msg = pretty_output(commit)
                if "Merge tag" not in msg:
                    print("commit", msg)


def main():
=======
        logging.error("Error: Cannot find the latest origin commit for %s", file_path)
        return

    if o_from_head["hash"] == o_from_t["hash"]:
        logging.debug("No update needed for %s", file_path)
    else:
        logging.info(file_path)
        commits = get_commits_count_between(
            opath, o_from_t["hash"], o_from_head["hash"]
        )
        count = 0
        for commit in commits:
            if valid_commit(commit):
                logging.info("commit %s", pretty_output(commit))
                count += 1
        logging.info("%d commits needs resolving in total\n", count)


def valid_locales(locale):
    """Check if the locale is valid or not"""
    script_path = os.path.dirname(os.path.abspath(__file__))
    linux_path = os.path.join(script_path, "..")
    if not os.path.isdir(f"{linux_path}/Documentation/translations/{locale}"):
        raise ArgumentTypeError("Invalid locale: {locale}")
    return locale


def list_files_with_excluding_folders(folder, exclude_folders, include_suffix):
    """List all files with the specified suffix in the folder and its subfolders"""
    files = []
    stack = [folder]

    while stack:
        pwd = stack.pop()
        # filter out the exclude folders
        if os.path.basename(pwd) in exclude_folders:
            continue
        # list all files and folders
        for item in os.listdir(pwd):
            ab_item = os.path.join(pwd, item)
            if os.path.isdir(ab_item):
                stack.append(ab_item)
            else:
                if ab_item.endswith(include_suffix):
                    files.append(ab_item)

    return files


class DmesgFormatter(logging.Formatter):
    """Custom dmesg logging formatter"""
    def format(self, record):
        timestamp = time.time()
        formatted_time = f"[{timestamp:>10.6f}]"
        log_message = f"{formatted_time} {record.getMessage()}"
        return log_message


def config_logging(log_level, log_file="checktransupdate.log"):
    """configure logging based on the log level"""
    # set up the root logger
    logger = logging.getLogger()
    logger.setLevel(log_level)

    # Create console handler
    console_handler = logging.StreamHandler()
    console_handler.setLevel(log_level)

    # Create file handler
    file_handler = logging.FileHandler(log_file)
    file_handler.setLevel(log_level)

    # Create formatter and add it to the handlers
    formatter = DmesgFormatter()
    console_handler.setFormatter(formatter)
    file_handler.setFormatter(formatter)

    # Add the handler to the logger
    logger.addHandler(console_handler)
    logger.addHandler(file_handler)


def main():
    """Main function of the script"""
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
    script_path = os.path.dirname(os.path.abspath(__file__))
    linux_path = os.path.join(script_path, "..")

    parser = ArgumentParser(description="Check the translation update")
    parser.add_argument(
        "-l",
        "--locale",
<<<<<<< HEAD
        help="Locale to check when files are not specified",
    )
    parser.add_argument(
        "--print-commits",
        action=BooleanOptionalAction,
        default=True,
        help="Print commits between the origin and the translation",
    )

    parser.add_argument(
        "--print-updated-files",
        action=BooleanOptionalAction,
        default=False,
        help="Print files that do no need to be updated",
    )

    parser.add_argument(
        "--debug",
        action=BooleanOptionalAction,
        help="Print debug information",
        default=False,
    )
=======
        default="zh_CN",
        type=valid_locales,
        help="Locale to check when files are not specified",
    )

    parser.add_argument(
        "--print-missing-translations",
        action=BooleanOptionalAction,
        default=True,
        help="Print files that do not have translations",
    )

    parser.add_argument(
        '--log',
        default='INFO',
        choices=['DEBUG', 'INFO', 'WARNING', 'ERROR', 'CRITICAL'],
        help='Set the logging level')

    parser.add_argument(
        '--logfile',
        default='checktransupdate.log',
        help='Set the logging file (default: checktransupdate.log)')
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

    parser.add_argument(
        "files", nargs="*", help="Files to check, if not specified, check all files"
    )
    args = parser.parse_args()

<<<<<<< HEAD
    global flag_p_c, flag_p_uf, flag_debug
    flag_p_c = args.print_commits
    flag_p_uf = args.print_updated_files
    flag_debug = args.debug

    # get files related to linux path
    files = args.files
    if len(files) == 0:
        if args.locale is not None:
            files = (
                os.popen(
                    "find {}/Documentation/translations/{} -type f".format(
                        linux_path, args.locale
                    )
                )
                .read()
                .split("\n")
            )
        else:
            files = (
                os.popen(
                    "find {}/Documentation/translations -type f".format(linux_path)
                )
                .read()
                .split("\n")
            )

    files = list(filter(lambda x: x != "", files))
=======
    # Configure logging based on the --log argument
    log_level = getattr(logging, args.log.upper(), logging.INFO)
    config_logging(log_level)

    # Get files related to linux path
    files = args.files
    if len(files) == 0:
        offical_files = list_files_with_excluding_folders(
            os.path.join(linux_path, "Documentation"), ["translations", "output"], "rst"
        )

        for file in offical_files:
            # split the path into parts
            path_parts = file.split(os.sep)
            # find the index of the "Documentation" directory
            kindex = path_parts.index("Documentation")
            # insert the translations and locale after the Documentation directory
            new_path_parts = path_parts[:kindex + 1] + ["translations", args.locale] \
                           + path_parts[kindex + 1 :]
            # join the path parts back together
            new_file = os.sep.join(new_path_parts)
            if os.path.isfile(new_file):
                files.append(new_file)
            else:
                if args.print_missing_translations:
                    logging.info(os.path.relpath(os.path.abspath(file), linux_path))
                    logging.info("No translation in the locale of %s\n", args.locale)

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
    files = list(map(lambda x: os.path.relpath(os.path.abspath(x), linux_path), files))

    # cd to linux root directory
    os.chdir(linux_path)

    for file in files:
        check_per_file(file)


if __name__ == "__main__":
    main()

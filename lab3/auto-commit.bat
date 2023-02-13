@echo off
:loop
    rem Check if the file has been updated
    git diff --quiet HEAD

    rem If the exit code is 1, the file has been updated
    if %errorlevel% == 1 (
        rem Add the changes to the repository
        git add .

        rem Commit the changes with a timestamp
        git commit -m "(auto-commit) lab 3 - %time%"

        rem Push the changes to the remote repository
        git push
    )

    rem Wait for a (time in seconds) before checking again
    timeout /t 5

goto loop
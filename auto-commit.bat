@echo off

if "%1"=="" (
    rem If no message was provided use a generic one
    set message=COMP 3430
) else (
    rem If a specific message was given use it
    set message="%1"
)

:loop
    rem Check if any files have been updated
    git diff --quiet HEAD

    rem If the exit code is 1, there are files that have been updated
    if %errorlevel% == 1 (
        rem Add all changes to the repository (including new files)
        git add -A

        rem Commit the changes with a timestamp
        git commit -m "%message% (auto-commit at %time%)"

        rem Push the changes to the remote repository
        git push
    )

    rem Wait for a (time in seconds) before checking again
    timeout /t 5

goto loop
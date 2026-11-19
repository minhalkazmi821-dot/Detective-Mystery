
#include <iostream>
#include <string>
using namespace std;

struct Suspect
{
    string name;
    string role;
    string details;
    int suspicion;
};

struct ClueInfo
{
    string description;
    int suspectIndex;
    int points;
};

struct Node
{
    int clueIndex;
    Node* next;
};

struct Action
{
    int clueIndex;
    int suspectIndex;
    int points;
};

const int MAX_SUSPECTS = 7;
const int MAX_CLUES = 10;
const int MAX_TASKS = 20;
const int MAX_ACTIONS = 20;

Suspect suspects[MAX_SUSPECTS];
ClueInfo clueCatalog[MAX_CLUES];

int suspectCount = 0;
int clueCount = 0;
int correctSuspect = -1;
int selectedLevel = 0;
int tasksCompleted = 0;

string caseTitle;
string caseStory;

Node* head = nullptr;

string tasks[MAX_TASKS];
int front = 0;
int rear = -1;

Action actionStack[MAX_ACTIONS];
int top = -1;

int readInt()
{
    int number;

    while (!(cin >> number))
    {
        cin.clear();
        cin.ignore(1000, '\n');   // cin.ignore used to prevent problems when taking the next input
        cout << "Enter a valid number: ";
    }

    cin.ignore(1000, '\n');

    return number;
}

string lowerCase(string text)
{
    for (string::size_type i = 0; i < text.length(); i++)
    {
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            text[i] = text[i] + 32;
        }
    }

    return text;
}

// The arry here stores the suspects.

void addSuspect(string name, string role, string details)
{
    if (suspectCount >= MAX_SUSPECTS)
    {
        return;
    }

    suspects[suspectCount].name = name;
    suspects[suspectCount].role = role;
    suspects[suspectCount].details = details;
    suspects[suspectCount].suspicion = 0;

    suspectCount++;
}

void displaySuspects()
{
    cout << "\n -/-/-/-/-/ SUSPECTS -/-/-/-/-/ \n";

    for (int i = 0; i < suspectCount; i++)
    {
        cout << i + 1 << ". " << suspects[i].name;
        cout << " (" << suspects[i].role << ")\n";
        cout << "   Suspicion: " << suspects[i].suspicion << "\n";
    }
}

void investigateSuspect()
{
    displaySuspects();

    cout << "\nEnter suspect number to investigate: ";
    int choice = readInt();

    if (choice < 1 || choice > suspectCount)
    {
        cout << "Invalid suspect number!\n";
        return;
    }

    cout << "\nInvestigation Report\n";
    cout << "Name: " << suspects[choice - 1].name << "\n";
    cout << "Role: " << suspects[choice - 1].role << "\n";
    cout << "Alibi: " << suspects[choice - 1].details << "\n";
}

// The linked list here stores collected clues
bool isClueCollected(int index)
{
    Node* current = head;

    while (current != nullptr)
    {
        if (current->clueIndex == index)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

void addClueNode(int index)
{
    Node* newNode = new Node;

    newNode->clueIndex = index;
    newNode->next = nullptr;

    if (head == nullptr)
    {
        head = newNode;
    }
    else
    {
        Node* current = head;

        while (current->next != nullptr)
        {
            current = current->next;
        }

        current->next = newNode;
    }
}

bool removeClueNode(int index)
{
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr)
    {
        if (current->clueIndex == index)
        {
            if (previous == nullptr)
            {
                head = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            delete current;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

void displayClues()
{
    cout << "\n -/-/-/-/-/ COLLECTED CLUES -/-/-/-/-/\n";

    if (head == nullptr)
    {
        cout << "No clues collected yet.\n";
        return;
    }

    Node* current = head;
    int number = 1;

    while (current != nullptr)
    {
        cout << number << ". ";
        cout << clueCatalog[current->clueIndex].description << "\n";

        current = current->next;
        number++;
    }
}

void addClue()
{
    cout << "\n -/-/-/-/-/ AVAILABLE CLUES -/-/-/-/-/\n";

    bool available = false;

    for (int i = 0; i < clueCount; i++)
    {
        if (!isClueCollected(i))
        {
            cout << i + 1 << ". ";
            cout << clueCatalog[i].description << "\n";
            available = true;
        }
    }

    if (!available)
    {
        cout << "All clues have been collected!\n";
        return;
    }

    cout << "\nEnter clue number to collect: ";
    int choice = readInt();

    if (choice < 1 || choice > clueCount)
    {
        cout << "Invalid clue number!\n";
        return;
    }

    int index = choice - 1;

    if (isClueCollected(index))
    {
        cout << "This clue is already collected!\n";
        return;
    }

    addClueNode(index);

    int suspectIndex = clueCatalog[index].suspectIndex;
    int points = clueCatalog[index].points;

    suspects[suspectIndex].suspicion += points;

    Action newAction;

    newAction.clueIndex = index;
    newAction.suspectIndex = suspectIndex;
    newAction.points = points;

    actionStack[++top] = newAction;

    cout << "\nClue collected successfully!\n";
    cout << "Suspicion increased for ";
    cout << suspects[suspectIndex].name << ".\n";
}

// The queueu is used here for the tasks because they are completed in the same order in which they are added

void addTask(string task)
{
    if (rear < MAX_TASKS - 1)
    {
        tasks[++rear] = task;
    }
}

void displayTasks()
{
    cout << "\n-/-/-/-/-/ INVESTIGATION TASKS -/-/-/-/-/\n";

    if (front > rear)
    {
        cout << "No pending tasks!\n";
        return;
    }

    for (int i = front; i <= rear; i++)
    {
        cout << i - front + 1 << ". " << tasks[i] << "\n";
    }
}

void completeTask()
{
    if (front > rear)
    {
        cout << "\nNo pending investigation tasks!\n";
        return;
    }

    cout << "\nCompleted Task: " << tasks[front] << "\n";

    front++;
    tasksCompleted++;

    cout << "Task completed successfully!\n";
}

// The stack is used for the undo operation
void undoAction()
{
    if (top == -1)
    {
        cout << "\nNothing to undo!\n";
        return;
    }

    Action lastAction = actionStack[top];

    removeClueNode(lastAction.clueIndex);

    suspects[lastAction.suspectIndex].suspicion -= lastAction.points;

    top--;

    cout << "\nLast clue collection has been undone!\n";
    cout << "Clue removed and suspicion score updated.\n";
}

// The linear search finds matching suspect names and clue
void searchSuspect()
{
    string searchName;

    cout << "\nEnter suspect name to search: ";
    getline(cin, searchName);

    bool found = false;

    for (int i = 0; i < suspectCount; i++)
    {
        if (lowerCase(suspects[i].name).find(lowerCase(searchName)) != string::npos)
        {
            cout << "\nSuspect Found!\n";
            cout << "Name: " << suspects[i].name << "\n";
            cout << "Role: " << suspects[i].role << "\n";
            cout << "Suspicion: " << suspects[i].suspicion << "\n";

            found = true;
        }
    }

    if (!found)
    {
        cout << "Suspect not found!\n";
    }
}

void searchClue()
{
    string searchText;

    cout << "\nEnter keyword to search in collected clues: ";
    getline(cin, searchText);

    Node* current = head;
    bool found = false;

    while (current != nullptr)
    {
        string description =
            clueCatalog[current->clueIndex].description;

        if (lowerCase(description).find(lowerCase(searchText)) != string::npos)
        {
            cout << "\nClue Found: " << description << "\n";
            found = true;
        }

        current = current->next;
    }

    if (!found)
    {
        cout << "Clue not found in your collected evidence!\n";
    }
}

// Bubble Sort ranks the suspects by suspicion score
void sortSuspects()
{
    Suspect sorted[7];

    for (int i = 0; i < suspectCount; i++)
    {
        sorted[i] = suspects[i];
    }

    for (int i = 0; i < suspectCount - 1; i++)
    {
        for (int j = 0; j < suspectCount - i - 1; j++)
        {
            if (sorted[j].suspicion < sorted[j + 1].suspicion)
            {
                Suspect temp = sorted[j];

                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    cout << "\n -/-/-/-/-/ SUSPECT RANKING -/-/-/-/-/ \n";

    for (int i = 0; i < suspectCount; i++)
    {
        cout << i + 1 << ". " << sorted[i].name;
        cout << " - Suspicion: " << sorted[i].suspicion << "\n";
    }
}

void setupLevel(int level)
{
    selectedLevel = level;

    suspectCount = 0;
    clueCount = 0;
    correctSuspect = -1;
    tasksCompleted = 0;

    front = 0;
    rear = -1;
    top = -1;

    if (level == 1)
    {
        caseTitle = "THE DISAPPEARANCE AT SHAADI NIGHT";

        caseStory =
            "During a crowded mehndi night at a family haveli in Lahore, "
            "a gold necklace belonging to the bride's mother disappears "
            "from a locked upstairs room. The family insists nobody from "
            "outside entered the house. Three people had reasons to hide "
            "something that night, and their stories do not fully match. "
            "Find out who took the necklace before the wedding celebrations end.";

        addSuspect("Murtaza", "Bride's Cousin",
                   "He says he was outside with the guests when the necklace disappeared. "
                   "Two cousins remember seeing him near the staircase, but disagree about the time.");

        addSuspect("Minhal", "Event Organizer",
                   "She says she was managing the mehndi stage and checking the sound system. "
                   "She had access to several rooms while arranging decorations.");

        addSuspect("Saadi", "Family Driver",
                   "He claims he stayed near the main gate all evening. "
                   "A guard remembers him leaving the gate area briefly, but Saadi says he was getting tea.");

        correctSuspect = 2;

        clueCatalog[0] = {
            "A broken piece of green glass is found near the upstairs window. "
            "Minhal had used green glass decorations, but the window was not broken from outside.",
            1, 15
        };

        clueCatalog[1] = {
            "A guest says Murtaza asked whether the upstairs room was locked earlier that evening. "
            "He claims he was only looking for a quiet place to make a phone call.",
            0, 15
        };

        clueCatalog[2] = {
            "A faint line of wet footprints leads from the back courtyard towards the servants' passage. "
            "The rain had stopped before the necklace was reported missing.",
            2, 20
        };

        clueCatalog[3] = {
            "A small piece of the necklace's red thread pouch is found in the decoration storage room. "
            "Minhal says many people had entered that room during the preparations.",
            1, 10
        };

        clueCatalog[4] = {
            "The gate register has an overwritten entry for 9:40 PM. "
            "Murtaza says the guard often makes mistakes when the house is crowded.",
            0, 10
        };

        clueCatalog[5] = {
            "The driver’s tea receipt is stamped at 9:55 PM, but the tea stall owner says Saadi collected it at 9:20 PM. "
            "This leaves an unexplained gap in Saadi's account.",
            2, 25
        };

        clueCatalog[6] = {
            "A muddy shoe mark near the servants' passage has the same unusual split pattern as the worn heel of Saadi's shoe. "
            "The mark alone does not prove when he was there.",
            2, 25
        };

        clueCount = 7;

        addTask("Inspect the upstairs room and its window");
        addTask("Question Murtaza about the staircase");
        addTask("Check the mehndi decoration storage");
        addTask("Visit the tea stall and verify Saadi's timing");
        addTask("Examine the servants' passage footprints");
    }

    else if (level == 2)
    {
        caseTitle = "THE LAST NIGHT AT THE DHA HOUSE";

        caseStory =
            "A well-known property dealer is found dead in his study during a dinner "
            "at his Karachi home. Earlier that evening, he announced that he would "
            "change the ownership of a valuable piece of land. Five people were in "
            "the house, and almost everyone had something to lose. A power cut, "
            "a delayed phone call, and a missing file make the timeline confusing. "
            "Separate the real evidence from the family gossip.";

        addSuspect("Murtaza", "Business Partner",
                   "He says he left the study before dinner to take a call. "
                   "He admits arguing about the land deal but says the disagreement was settled.");

        addSuspect("Minhal", "Daughter",
                   "She says she was on the terrace speaking to a friend. "
                   "She knew about the planned change in the property papers.");

        addSuspect("Saadi", "House Manager",
                   "He claims he was arranging dinner with the kitchen staff. "
                   "He had keys to several rooms but says the study key was never with him.");

        addSuspect("Mohid", "Family Lawyer",
                   "He says he was reviewing documents in the guest room. "
                   "He had prepared the papers related to the property transfer.");

        addSuspect("Sarim", "Security Guard",
                   "He claims he stayed at the main entrance throughout the evening. "
                   "The security camera stopped recording for a few minutes during the power cut.");

        correctSuspect = 3;

        clueCatalog[0] = {
            "A draft property agreement shows Murtaza would lose a large share of the land if the transfer went ahead. "
            "The draft is unsigned, so it does not prove he knew the final decision.",
            0, 20
        };

        clueCatalog[1] = {
            "A voice note captures Minhal saying, 'Tonight everything will change.' "
            "Her friend says they were discussing a surprise birthday plan.",
            1, 15
        };

        clueCatalog[2] = {
            "The kitchen staff says Saadi left the dining area for nearly ten minutes. "
            "He insists he went to check the water supply during the power cut.",
            2, 15
        };

        clueCatalog[3] = {
            "A page from the property file is missing, and the remaining pages carry Mohid's handwritten corrections. "
            "He says the corrections were made during normal legal work.",
            3, 20
        };

        clueCatalog[4] = {
            "The security camera clock is seven minutes ahead of the wall clock. "
            "This makes the recorded movement near the study look later than it actually was.",
            4, 10
        };

        clueCatalog[5] = {
            "A restaurant receipt places Murtaza outside the house, but the time is handwritten and not machine printed. "
            "The restaurant worker cannot confirm who collected the order.",
            0, 10
        };

        clueCatalog[6] = {
            "A call log shows Mohid received a call from the victim shortly before the estimated time of death. "
            "Mohid first says they did not speak that night, then claims the call was only about paperwork.",
            3, 25
        };

        clueCatalog[7] = {
            "A torn corner of the missing file is found inside the folder Mohid brought to the house. "
            "The paper matches the file, but the folder had been left unattended during dinner.",
            3, 25
        };

        clueCatalog[8] = {
            "A faint mark on the study door suggests it was pulled shut with a piece of cloth. "
            "The cloth fibres match the lining of the lawyer's document bag, although the bag was handled by others.",
            3, 20
        };

        clueCount = 9;

        addTask("Inspect the study and check the estimated time of death");
        addTask("Question Murtaza about the land agreement");
        addTask("Verify Minhal's voice note");
        addTask("Check the security camera and wall clock times");
        addTask("Examine the missing property file");
        addTask("Question Mohid about the phone call");
        addTask("Compare the fibres found on the study door");
    }

    else if (level == 3)
    {
        caseTitle = "THE SECRET OF THE OLD CITY";

        caseStory =
            "A respected community leader is found dead inside a historic haveli "
            "in Lahore after a private meeting about a charity fund. The fund's "
            "records have disappeared, and several people could benefit from keeping "
            "them hidden. A planned power shutdown, a switched visitor register, "
            "and conflicting witness statements make this the most difficult case "
            "so far. Every suspect is hiding something, but only one is hiding a murder.";

        addSuspect("Murtaza", "Charity Treasurer",
                   "He says he was counting donations in the main hall. "
                   "Several entries in the fund ledger were written in his handwriting, but he says he was correcting old mistakes.");

        addSuspect("Minhal", "Journalist",
                   "She claims she came to interview the community leader and left before the meeting ended. "
                   "Her recorder contains a conversation that she refuses to explain.");

        addSuspect("Saadi", "Caretaker",
                   "He says he locked the courtyard after the guests arrived. "
                   "He knows the old passages of the haveli and says one side door has been broken for months.");

        addSuspect("Mohid", "Political Adviser",
                   "He says he was on a phone call in the library. "
                   "The victim had recently threatened to expose a private agreement involving him.");

        addSuspect("Sarim", "Security Supervisor",
                   "He claims the power shutdown was scheduled and that all guards followed the usual procedure. "
                   "His duty sheet has a correction made in a different ink.");

        addSuspect("Anas", "Personal Assistant",
                   "He says he prepared the victim's tea and then went to collect a document from the office. "
                   "He had access to the victim's schedule and personal belongings.");

        addSuspect("Ubaid", "Accountant",
                   "He says he discovered irregularities in the charity accounts and informed the victim. "
                   "He claims he left the haveli before the power shutdown.");

        correctSuspect = 5;

        clueCatalog[0] = {
            "Several charity entries were changed shortly before the meeting. "
            "The handwriting resembles Murtaza's, but the ink is different from the pen found at his desk.",
            0, 15
        };

        clueCatalog[1] = {
            "Minhal's recorder contains a cut in the middle of an interview. "
            "She says she removed a private conversation to protect a source.",
            1, 15
        };

        clueCatalog[2] = {
            "Dust near the old side door has been disturbed, although Saadi says nobody has used that passage for weeks. "
            "The disturbance could have been caused during the afternoon cleaning.",
            2, 15
        };

        clueCatalog[3] = {
            "A folded note in the library mentions a secret agreement involving Mohid. "
            "The note has no signature and may have been written to mislead the investigation.",
            3, 20
        };

        clueCatalog[4] = {
            "The guard duty sheet shows Sarim changed the power-shutdown time from 10:00 PM to 10:20 PM. "
            "He says the electricity department changed its schedule.",
            4, 20
        };

        clueCatalog[5] = {
            "Ubaid's account book shows a missing payment, but the amount was transferred back the following morning. "
            "He says the victim had asked him to keep the transfer quiet.",
            6, 15
        };

        clueCatalog[6] = {
            "The victim's schedule was edited on the same afternoon, removing a meeting with an unknown visitor. "
            "Anas says he made the change on the victim's instructions.",
            5, 20
        };

        clueCatalog[7] = {
            "A tea cup from the victim's room has a faint stain matching the herbal mixture kept in the assistant's desk. "
            "The mixture was also served to guests earlier that evening.",
            5, 20
        };

        clueCatalog[8] = {
            "A message recovered from the victim's phone says, 'Bring the original register, not the copy.' "
            "The message was sent to Anas, who claims he never collected any register.",
            5, 25
        };

        clueCatalog[9] = {
            "A torn strip from the missing visitor register is found inside the cover of Anas's notebook. "
            "Its torn edges fit the remaining register, although the notebook had been left in the shared office.",
            5, 25
        };

        clueCount = 10;

        addTask("Inspect the victim's room and the tea cup");
        addTask("Compare the charity ledger entries");
        addTask("Question Minhal about her recording");
        addTask("Inspect the old side passage");
        addTask("Verify the power shutdown with the electricity schedule");
        addTask("Examine the missing visitor register");
        addTask("Question Anas about the victim's schedule and messages");
        addTask("Cross-check the charity account with Ubaid");
    }
}

// The final accusation, where the program checks the selected suspect and calculates the score

int getCollectedCount()
{
    int count = 0;
    Node* current = head;

    while (current != nullptr)
    {
        count++;
        current = current->next;
    }

    return count;
}

bool finalAccusation()
{
    string name;

    cout << "\n -/-/-/-/-/ FINAL ACCUSATION -/-/-/-/-/ \n";

    displaySuspects();

    cout << "\nEnter the name of the person you believe is the killer: ";
    getline(cin, name);

    int accused = -1;

    for (int i = 0; i < suspectCount; i++)
    {
        if (lowerCase(suspects[i].name) == lowerCase(name))
        {
            accused = i;
            break;
        }
    }

    if (accused == -1)
    {
        cout << "Invalid suspect name. Try again.\n";
        return false;
    }

    if (accused == correctSuspect)
    {
        int score = 50 + getCollectedCount() * 10
                    + tasksCompleted * 5;

        cout << "\n*** CONGRATULATIONS! Detective! ***\n";
        cout << "You solved the mystery!\n";
        cout << "The killer was " << suspects[correctSuspect].name << ".\n";
        cout << "Your final score: " << score << "\n";

        cout << "\nCASE SUCCESSFULLY SOLVED!\n";

        return true;
    }

    else
    {
        cout << "\nWRONG ACCUSATION!\n";
        cout << "The actual killer was ";
        cout << suspects[correctSuspect].name << ".\n";

        cout << "The case remains unsolved.\n";

        return true;
    }
}

// GAME MENU

void playGame()
{
    cout << "\n|---------------------------------|\n";
    cout << "       " << caseTitle << "\n";
    cout << "|----------------------------------|\n";

    cout << "\n" << caseStory << "\n";

    cout << "\nYou must investigate and find the truth!\n";

    while (true)
    {
        cout << "\n-/-/-/-/-/ INVESTIGATION MENU -/-/-/-/-/\n";

        cout << "1. View All Suspects\n";
        cout << "2. Investigate a Suspect\n";
        cout << "3. Collect a Clue\n";
        cout << "4. View Collected Clues\n";
        cout << "5. View Investigation Tasks\n";
        cout << "6. Complete Next Task\n";
        cout << "7. Undo Last Action\n";
        cout << "8. Search Suspect\n";
        cout << "9. Search Collected Clue\n";
        cout << "10. Rank Suspects by Suspicion\n";
        cout << "11. Accuse the Killer\n";
        cout << "12. Abandon the Case\n";

        cout << "\nEnter your choice: ";

        int choice = readInt();

        switch (choice)
        {
            case 1:
                displaySuspects();
                break;

            case 2:
                investigateSuspect();
                break;

            case 3:
                addClue();
                break;

            case 4:
                displayClues();
                break;

            case 5:
                displayTasks();
                break;

            case 6:
                completeTask();
                break;

            case 7:
                undoAction();
                break;

            case 8:
                searchSuspect();
                break;

            case 9:
                searchClue();
                break;

            case 10:
                sortSuspects();
                break;

            case 11:
                if (finalAccusation())
                {
                    return;
                }
                break;

            case 12:
                cout << "\nYou abandoned the investigation.\n";
                return;

            default:
                cout << "\nInvalid choice! Try again.\n";
        }
    }
}

// INSTRUCTIONS

void instructions()
{
    cout << "\n-/-/-/-/-/ GAME INSTRUCTIONS -/-/-/-/-/\n";

    cout << "1. Select a difficulty level.\n";
    cout << "2. Investigate the suspects.\n";
    cout << "3. Collect clues to increase suspicion scores.\n";
    cout << "4. Complete investigation tasks using the queue.\n";
    cout << "5. Use undo to remove your last collected clue.\n";
    cout << "6. Search for suspects and collected clues.\n";
    cout << "7. Sort suspects according to suspicion scores.\n";
    cout << "8. Make your final accusation.\n";

    cout << "\nCorrect accusation gives you 50 points.\n";
    cout << "Each collected clue gives you 10 points.\n";
    cout << "Each completed task gives you 5 points.\n";
}

void clearClues()
{
    Node* current = head;

    while (current != nullptr)
    {
        Node* temp = current;
        current = current->next;
        delete temp;
    }

    head = nullptr;
}

int main()
{
    while (true)
    {
        cout << "\n|---------------------------------|\n";
        cout << "       DETECTIVE MYSTERY\n";
        cout << "        WHO IS THE KILLER?\n";
        cout << "|---------------------------------|\n";

        cout << "\n1. Start New Game\n";
        cout << "2. View Instructions\n";
        cout << "3. Exit Game\n";

        cout << "\nEnter your choice: ";

        int choice = readInt();

        if (choice == 1)
        {
            cout << "\nSelect Difficulty Level:\n";
            cout << "1. Easy - The Missing Necklace\n";
            cout << "2. Medium - Murder at Midnight\n";
            cout << "3. Hard - The Final Conspiracy\n";

            cout << "\nEnter level: ";

            int level = readInt();

            if (level < 1 || level > 3)
            {
                cout << "Invalid level!\n";
                continue;
            }

            clearClues();

            setupLevel(level);

            playGame();

            clearClues();
        }

        else if (choice == 2)
        {
            instructions();
        }

        else if (choice == 3)
        {
            clearClues();

            cout << "\nThank you for playing Detective Mystery!\n";
            break;
        }

        else
        {
            cout << "\nInvalid choice! Try again.\n";
        }
    }
    return 0;
}

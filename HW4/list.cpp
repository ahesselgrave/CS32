void listAll(const Class* c, string path)  // two-parameter overload
{
    cout << path + c->name() << endl;
    for(vector<Class*>::const_iterator itr = c->subclasses().begin(); itr != c->subclasses().end(); itr++)
    {
        string next = path + c->name() + "=>";
        listAll((*itr), next);
    }
}
